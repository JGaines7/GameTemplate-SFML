#ifndef SIMULATION_H
#define SIMULATION_H

#include <SFML/Graphics.hpp>
#include "Human.h"
#include "Zombie.h"
#include "QuadTree.h"

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

        const std::vector<std::unique_ptr<Zombie>>& getZombies() const { return m_zombies;}
        const std::vector<std::unique_ptr<Human>>& getHumans() const { return m_humans;}

         QuadTree* getQuadTree(std::string key)  { return m_quadTrees[key].get();}


    protected:

    private:

        std::vector<std::unique_ptr<Zombie>> m_zombies;
        std::vector<std::unique_ptr<Human>> m_humans;

        sf::Vector2f m_target;
        SimulationSettings m_settings;

        std::map<std::string, std::unique_ptr<QuadTree>> m_quadTrees;

        unsigned int m_currentUpdateGroup = 0;
        int nextUpdateGroup() {m_currentUpdateGroup = (m_currentUpdateGroup + 1) % m_maxGroups; return m_currentUpdateGroup;}


        const unsigned int m_maxGroups = 5;

        //Map m_map

        //helpers
        sf::Vector2f randomDrift(float factor);
        void handleCollisions();
        void handleBorderCollisions();
        void handleBorderCollision(Entity& ent, float rad);
        void checkKills();

        void randomSpawnSettings(Entity& ent);

        //TODO DEMO texture
        sf::Texture m_demoTexture;


};

#endif // SIMULATION_H
