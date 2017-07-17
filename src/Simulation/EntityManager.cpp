#include "EntityManager.h"
#include <iostream>
EntityManager::EntityManager(sf::Rect<int> worldRect) : m_worldRect(worldRect)
{

}

EntityManager::~EntityManager()
{
    //dtor
}


void EntityManager::addEntity(EntityType entityType, std::unique_ptr<Entity>& entity)
{
    insertEntityIntoQuadTree(entityType, entity.get());
    m_entityMap[entityType].push_back(std::move(entity));

}

void EntityManager::destroyEntity(EntityType entityType, Entity* entity)
{
    //search for entity in vector, remove it.
    //TODO search for pointer in quad tree( using its own rect property to search )
    m_quadTrees.at(entityType)->remove(entity);

    for(auto& entityPtr : m_entityMap.at(entityType))
    {
        if(entityPtr.get() == entity)
        {
            std::swap(entityPtr, m_entityMap.at(entityType).back());
            m_entityMap.at(entityType).pop_back();
            break;
        }
    }


}

std::vector<Entity*> const EntityManager::getEntityList(EntityType entType) const
{
    //convert vector of unique ptr to raw.
    //This is kind inefficient but ideally we aren't calling this function appreciably often


    std::vector<Entity*> retVal;

    try
    {
        auto& entityUniquePtrList = m_entityMap.at(entType);
        retVal.reserve(entityUniquePtrList.size());
        for(auto& entityUniquePtr : entityUniquePtrList)
        {
            retVal.push_back(entityUniquePtr.get());
        }
    }
    catch(std::out_of_range)
    {
        std::cout << "Entity doesnt exist!\n";
    }
    return retVal;
}

const QuadTree* EntityManager::getQuadTree(EntityType entType) const
{
    QuadTree* retVal = nullptr;
    if(m_quadTrees.find(entType) != m_quadTrees.end())
    {
        retVal =  m_quadTrees.at(entType).get();
    }
    return retVal;
}


//Searches quad tree of type entType for entities that 'may' intersect with filterRect.
std::vector<Entity*> EntityManager::getNearbyEntitiesFromQuad(EntityType entType, sf::Rect<float> filterRect)
{
     std::vector<Entity*> retVal;

     if(m_quadTrees.find(entType) != m_quadTrees.end())
    {
        retVal =  m_quadTrees.at(entType)->retrieve(filterRect);
    }

    return retVal;
}


void EntityManager::rebuildQuadTree()
{
    //for each

    for(auto& entry :  m_entityMap)
    {
        m_quadTrees.erase(entry.first);

        for(auto& ent : entry.second)
        {
            insertEntityIntoQuadTree(entry.first, ent.get());
        }
    }
}

void EntityManager::insertEntityIntoQuadTree(EntityType entityType, Entity* entity)
{
    if(m_quadTrees[entityType] == nullptr)
    {
        m_quadTrees[entityType].reset(new QuadTree(0, m_worldRect));
    }
    m_quadTrees[entityType]->insert(entity);
}

void EntityManager::reset(sf::Rect<int> worldDimensions)
{
    m_entityMap.clear();
    m_quadTrees.clear();
    m_worldRect = worldDimensions;

}
