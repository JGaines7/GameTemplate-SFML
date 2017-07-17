#include "QuadTree.h"
#include <iostream>
#include "VectorUtilities.h"

QuadTree::QuadTree(int level, sf::Rect<int> bounds)
{
    m_level = level;
    m_boundingRect = bounds;

    for(int i = 0; i < 4; i++)
    {
        m_childNodes[i] = nullptr;
    }

}

QuadTree::~QuadTree()
{
    //dtor
}

void QuadTree::clear()
{
    m_entityList.clear();
    for(int i = 0; i < 4; i++)
    {
        if(m_childNodes[i] != nullptr)
        {
            m_childNodes[i]->clear();
        }
    }
}

void QuadTree::split()
{
    int subWidth = m_boundingRect.width >> 1;
    int subHeight = m_boundingRect.height >> 1;
    int x = m_boundingRect.left;
    int y = m_boundingRect.top;

    m_childNodes[0].reset( new QuadTree(m_level+1, sf::Rect<int>(x , y, subWidth, subHeight)));
    m_childNodes[1].reset( new QuadTree(m_level+1, sf::Rect<int>(x + subWidth, y, subWidth, subHeight)));
    m_childNodes[2].reset( new QuadTree(m_level+1, sf::Rect<int>(x + subWidth, y + subHeight, subWidth, subHeight)));
    m_childNodes[3].reset( new QuadTree(m_level+1, sf::Rect<int>(x, y + subHeight, subWidth, subHeight)));

}

/*
 * Determine which node the object belongs to. -1 means
 * object cannot completely fit within a child node and is part
 * of the parent node
 */
std::vector<int> QuadTree::findEncompassingNode(sf::Rect<float> inputRect) const
{
    std::vector<int> returnVector;
    int horizontalMidpoint = m_boundingRect.left + (m_boundingRect.width / 2);
    int verticalMidpoint = m_boundingRect.top + (m_boundingRect.height / 2);

    int subQuadrantSize = m_boundingRect.width / 2;

    sf::Rect<int> quadrantRect(m_boundingRect.left, m_boundingRect.top, subQuadrantSize, subQuadrantSize);
    if(VectorUtil::parallelRectIntersect(inputRect, quadrantRect)) returnVector.push_back(0);
    quadrantRect = sf::Rect<int>(horizontalMidpoint, m_boundingRect.top, subQuadrantSize, subQuadrantSize);
    if(VectorUtil::parallelRectIntersect(inputRect, quadrantRect)) returnVector.push_back(1);
    quadrantRect = sf::Rect<int>(horizontalMidpoint, verticalMidpoint, subQuadrantSize, subQuadrantSize);
    if(VectorUtil::parallelRectIntersect(inputRect, quadrantRect)) returnVector.push_back(2);
    quadrantRect = sf::Rect<int>(m_boundingRect.left, verticalMidpoint, subQuadrantSize, subQuadrantSize);
    if(VectorUtil::parallelRectIntersect(inputRect, quadrantRect)) returnVector.push_back(3);

    return returnVector;
}

/*
 * Insert the object into the quadtree. If a node subsequently
 * exceeds the capacity, it will split and add all
 * objects to their corresponding nodes.
 */
void QuadTree::insert( Entity* entityPtr)
{
    if (m_childNodes[0] != nullptr)
    {

        std::vector<int> indices = findEncompassingNode(entityPtr->getGlobalBounds());

        for(int index : indices)
        {
            m_childNodes[index]->insert(entityPtr);
            return;
        }
    }

    m_entityList.push_back(entityPtr);

    if (m_entityList.size() > MAX_OBJECTS && m_level < MAX_LEVELS)
    {
        if (m_childNodes[0] == nullptr)
        {
            split();
        }

        int j = 0;
        for(unsigned int i = 0; i < m_entityList.size(); i++)
        {
            std::vector<int> indices = findEncompassingNode(m_entityList[i]->getGlobalBounds());
            for(int index : indices)
            {
                m_childNodes[index]->insert(m_entityList[i]);
                j++;
            }
        }
        //We are no longer a leaf node. All previous entities should now have been added to new leaf nodes.
        m_entityList.clear();

    }

}

std::vector<Entity*> QuadTree::retrieve( sf::Rect<float> inputRect) const
{

    std::vector<Entity*> returnVector;

    //if not a leaf node, recurse into leaf nodes
    if(m_childNodes[0] != nullptr)
    {
        std::vector<int> indices  = findEncompassingNode(inputRect);
        for(int index : indices)
        {
            //append found entities in children nodes.
            std::vector<Entity*> foundEntities = m_childNodes[index]->retrieve( inputRect);
            //we may get duplicates however, so search returnVector for duplicates first.
            for (auto foundEntity : foundEntities)
            {
                bool duplicate = false;
                for(auto& existingEntity : returnVector)
                {
                    if(existingEntity == foundEntity)
                    {
                        duplicate = true;
                        break;
                    }
                }
                if(!duplicate)
                {
                    returnVector.push_back(foundEntity);
                }
            }

        }

    }
    else
    {
        //this is a leaf node, return its
        returnVector = m_entityList;
    }

    return returnVector;
}

void QuadTree::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

    //draw every node
    if (m_childNodes[0] != nullptr)
    {
        for(int i = 0; i < 4; i++)
        {
            m_childNodes[i]->draw( target,  states);
        }
    }

    //draw ourselves
    sf::Vector2f boundingDim(m_boundingRect.width, m_boundingRect.height);
    sf::Vector2f boundingPos(m_boundingRect.left, m_boundingRect.top);

    auto col = sf::Color(255-m_level*124,255-m_level*555,255-m_level*151, 40);


    sf::VertexArray border(sf::LinesStrip, 0);
    border.append(sf::Vertex(sf::Vector2f(boundingPos.x,boundingPos.y), col));
    border.append(sf::Vertex(sf::Vector2f(boundingPos.x + boundingDim.x,boundingPos.y), col));
    border.append(sf::Vertex(sf::Vector2f(boundingPos.x + boundingDim.x,boundingPos.y + boundingDim.y), col));
    border.append(sf::Vertex(sf::Vector2f(boundingPos.x,boundingPos.y + boundingDim.y), col));
    border.append(sf::Vertex(sf::Vector2f(boundingPos.x,boundingPos.y), col));
    target.draw(border);



}

void QuadTree::remove(Entity* entityPtr)
{
    sf::Rect<float> searchRect = entityPtr->getGlobalBounds();
    searchRect.width += 100;
    searchRect.top -= 50;
    searchRect.left -= 50;
    remove_recurse(entityPtr, searchRect);
}

void QuadTree::remove_recurse(Entity* entPtr, sf::Rect<float> inputRect)
{

    if(m_childNodes[0] != nullptr)
    {
        std::vector<int> indices  = findEncompassingNode(inputRect);
        for(int index : indices)
        {
            m_childNodes[index]->remove_recurse(entPtr, inputRect);
        }
    }
    else
    {
        //this is a leaf node, return its
        for(auto& ptr : m_entityList)
        {
            if(ptr == entPtr)
            {
                std::swap(ptr, m_entityList.back());
                m_entityList.pop_back();
            }
        }
    }
}
