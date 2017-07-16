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
    //I think just deleting this should clean up child nodes correctly
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
    //std::cout << "Split\n";
    ////std::cout << "splitting node: xy" << m_boundingRect.left << "," << m_boundingRect.top << "Size: " << m_boundingRect.width << "," << m_boundingRect.height << "\n";
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
    //std::cout << "find encompassing nodes for inputrect: " << inputRect.left << ":" << inputRect.top << "\n";
    std::vector<int> returnVector;
    int horizontalMidpoint = m_boundingRect.left + (m_boundingRect.width / 2);
    int verticalMidpoint = m_boundingRect.top + (m_boundingRect.height / 2);

    int subQuadrantSize = m_boundingRect.width / 2;

    //(X1+W1<X2 or X2+W2<X1 or Y1+H1<Y2 or Y2+H2<Y1):
    //does inputRect touch leftQuadrants?
    sf::Rect<int> quadrantRect(m_boundingRect.left, m_boundingRect.top, subQuadrantSize, subQuadrantSize);
    if(VectorUtil::parallelRectIntersect(inputRect, quadrantRect)) returnVector.push_back(0);
    quadrantRect = sf::Rect<int>(horizontalMidpoint, m_boundingRect.top, subQuadrantSize, subQuadrantSize);
    if(VectorUtil::parallelRectIntersect(inputRect, quadrantRect)) returnVector.push_back(1);
    quadrantRect = sf::Rect<int>(horizontalMidpoint, verticalMidpoint, subQuadrantSize, subQuadrantSize);
    if(VectorUtil::parallelRectIntersect(inputRect, quadrantRect)) returnVector.push_back(2);
    quadrantRect = sf::Rect<int>(m_boundingRect.left, verticalMidpoint, subQuadrantSize, subQuadrantSize);
    if(VectorUtil::parallelRectIntersect(inputRect, quadrantRect)) returnVector.push_back(3);


//   // Object can completely fit within the top quadrants
//   bool topQuadrant = (inputRect.top < verticalMidpoint && inputRect.top + inputRect.height < verticalMidpoint);
//   // Object can completely fit within the bottom quadrants
//   bool bottomQuadrant = (inputRect.top > verticalMidpoint);
//
//   // Object can completely fit within the left quadrants
//   if (inputRect.left < horizontalMidpoint && inputRect.left + inputRect.width < horizontalMidpoint) {
//      if (topQuadrant) {
//        index = 0;
//      }
//      else if (bottomQuadrant) {
//        index = 3;
//      }
//    }
//    // Object can completely fit within the right quadrants
//    else if (inputRect.left > horizontalMidpoint) {
//     if (topQuadrant) {
//       index = 1;
//     }
//     else if (bottomQuadrant) {
//       index = 2;
//     }
//   }

    //std::cout << "Encompasses " << returnVector.size() << "nodes\n";
   return returnVector;
}

/*
 * Insert the object into the quadtree. If the node
 * exceeds the capacity, it will split and add all
 * objects to their corresponding nodes.
 */
void QuadTree::insert( Entity* entityPtr)
{
    //std::cout << "Insert entity at position: " << entityPtr->getGlobalBounds().left << ":" << entityPtr->getGlobalBounds().top << "\n";
    if (m_childNodes[0] != nullptr)
    {

        std::vector<int> indices = findEncompassingNode(entityPtr->getGlobalBounds());


        //this should instead be a list of nodes this entity touches.

        //for each index, insert our pointer into it.
        for(int index : indices)
        {
            //std::cout << "This entity appears to encompass node: " << index << "\n";

            m_childNodes[index]->insert(entityPtr);
            return;
        }
    }

    m_entityList.push_back(entityPtr);

    //std::cout << "m_entityList.szie: " << m_entityList.size() << "\n";
    if (m_entityList.size() > MAX_OBJECTS && m_level < MAX_LEVELS)
    {
        //std::cout << "Need to split! level: "<< m_level << "\n";
        if (m_childNodes[0] == nullptr)
        {
            ////std::cout << "split first!\n";
            split();
        }

        int j = 0;
        for(unsigned int i = 0; i < m_entityList.size(); i++)
        {
            std::vector<int> indices = findEncompassingNode(m_entityList[i]->getGlobalBounds());
            ////std::cout << "existing element fits in: " << index << "\n";
            for(int index : indices)
            {
                m_childNodes[index]->insert(m_entityList[i]);
//                std::swap(m_entityList[index], m_entityList.back());
                j++;
            }
        }
        //We are no longer a leaf node. All previous entities should now have been added to new leaf nodes.
        m_entityList.clear();
        //std::cout << "entity list for this node is now size : " << m_entityList.size() << "\n";
    }

}

std::vector<Entity*> QuadTree::retrieve( sf::Rect<float> inputRect) const
{

    std::vector<Entity*> returnVector;

    //for each index in findencompassingNode, add to the return vector;
//    if (index != -1 && m_childNodes[0] != nullptr)
//    {
//        returnVector = m_childNodes[index]->retrieve( inputRect);
//    }

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
        returnVector = std::move(m_entityList);
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


    //target.draw(nodeRect);


}

//bool QuadTree::rectanglesIntersect(sf::Rect<float>& r1, sf::Rect<int>& r2)
//{
//    if((r1.left + r1.width < r2.left) ||
//       (r2.left + r2.width < r1.left) ||
//       (r1.top + r1.h < r2.top) ||
//       (r2.top + r2.height < r1.top))
//    {
//        return true;
//    }
//    return false;
//}
//
