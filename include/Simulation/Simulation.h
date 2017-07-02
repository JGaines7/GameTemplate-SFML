#ifndef SIMULATION_H
#define SIMULATION_H

#include <SFML/Graphics.hpp>
#include "Human.h"
#include "Zombie.h"

//Simulation holds everything relevant to running a simulation. Map, configuration, zombie/human positions, etc.
//This class should stay agnostic to any rendering details.
class Simulation
{
    public:
        Simulation();
        virtual ~Simulation();

        //loadSimulation (config + tilemap)
        //saveSimulation

        void stepSimulation();

        void startSimulation();

        const sf::Vector2i getWorldSize() const {return m_worldDimensions;} //when we get more of these, make a configuration struct

        const std::vector<Zombie>& getZombies() const { return m_zombies;}
        const std::vector<Human>& getHumans() const { return m_humans;}


    protected:

    private:

        //std::vector<Human> m_humans;
        std::vector<Zombie> m_zombies;
        std::vector<Human> m_humans;

        sf::Vector2f m_target;

        //SimConfiguration m_configuration
        sf::Vector2i m_worldDimensions;

        unsigned int m_currentUpdateGroup = 0;
        int nextUpdateGroup() {m_currentUpdateGroup = (m_currentUpdateGroup + 1) % m_maxGroups; return m_currentUpdateGroup;}


        const unsigned int m_maxGroups = 10;

        //Map m_map

        //helpers
        sf::Vector2f randomDrift();
        void handleCollisions();
        void handleBorderCollisions();
        void handleBorderCollision(Entity& ent);
        void checkKills();

        void randomSpawnSettings(Entity& ent);


};

#endif // SIMULATION_H
