#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Entity.h"
#include "Human.h"
#include "Zombie.h"
#include "QuadTree.h"
#include "EntityTypes.h"
#include <SFML/Graphics.hpp>
//EntityManager provides an interface for adding and removing entities from the simulation.
//This hides most of the implementation details of the quad tree and entity vectors so that we cant muck it up.

//When deleting an object we search the quad tree based on the entity's position + 50 radius.  This implies a speed limit of 50 pixels per tick or else risk failing to delete from quad tree.


class EntityManager
{
    public:
        EntityManager(sf::Rect<int> worldRect);
        virtual ~EntityManager();

        void addEntity(EntityType entityType, std::unique_ptr<Entity>& entity);
        void destroyEntity(EntityType entityType, Entity* entity);
        //return const ref to entity list
        const std::vector<Entity*>  getEntityList(EntityType entType) const;
        const QuadTree* getQuadTree(EntityType entType) const;

        void reset(sf::Rect<int> worldDimensions);
        //Searches quad tree of type entType for entities that 'may' intersect with filterRect.
        std::vector<Entity*> getNearbyEntitiesFromQuad(EntityType entType, sf::Rect<float> filterRect);
        //TODO: getIntersectingEntities(EntityType entType, sf::Rect filterRect);  //Handle the second level collision detection directly.

        void rebuildQuadTree();


    protected:

    private:
        std::map<EntityType, std::vector<std::unique_ptr<Entity>>> m_entityMap;

        std::map<EntityType, std::unique_ptr<QuadTree>> m_quadTrees;

        void insertEntityIntoQuadTree(EntityType entityType, Entity* entity);

        sf::Rect<int> m_worldRect; //Need to know world dimensions to manage quadtrees.


};

#endif // ENTITYMANAGER_H
