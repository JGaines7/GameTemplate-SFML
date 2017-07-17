#ifndef SIMULATION_H
#define SIMULATION_H

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Human.h"
#include "Zombie.h"
#include "QuadTree.h"
#include "EntityManager.h"
#include "EntityTypes.h"

class Entity;
//Simulation holds everything relevant to running a simulation. Map, configuration, zombie/human positions, etc.
//This class should stay agnostic to any rendering details.
class Simulation
{
    public:
        Simulation();
        virtual ~Simulation();



        //loadSimulation (config + tilemap)
        //saveSimulation

        const SimulationSettings getSimulationSettings() const {return m_settings;}
        void setSimulationSettings(SimulationSettings settings) { m_settings = settings;}

        void stepSimulation();
        void addHuman(sf::Vector2f position);

        void startSimulation();

        const std::vector<Entity*> getZombies() const { return m_entityManager.getEntityList(EntityType::ENT_ZOMBIE);}
        const std::vector<Entity*> getHumans() const { return m_entityManager.getEntityList(EntityType::ENT_HUMAN);}

        const QuadTree* getQuadTree(EntityType type) const { return m_entityManager.getQuadTree(type);}


    protected:

    private:

        EntityManager m_entityManager;

        SimulationSettings m_settings;

        unsigned int m_currentUpdateGroup = 0;
        const unsigned int m_maxGroups = 5;
        int nextUpdateGroup() {m_currentUpdateGroup = (m_currentUpdateGroup + 1) % m_maxGroups; return m_currentUpdateGroup;}

        //helpers
        sf::Vector2f randomDrift(float factor);
        void handleCollisions();
        void handleBorderCollisions();
        void handleBorderCollision(Entity& ent, float rad);
        void checkKills();
        void moveEntities();

        void randomSpawnSettings(Entity& ent);

        //TODO DEMO texture
        sf::Texture m_demoTexture;


};

#endif // SIMULATION_H
