#ifndef SIMULATION_H
#define SIMULATION_H

#include <SFML/Graphics.hpp>
//#include "Human.h"
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
        void setTargetPos(sf::Vector2f pos);

        const std::vector<Zombie>& getZombies() const { return m_zombies;}


    protected:

    private:

        //std::vector<Human> m_humans;
        std::vector<Zombie> m_zombies;

        sf::Vector2f m_target;

        //SimConfiguration m_configuration
        int m_worldWidth = 1000;
        int m_worldHeight = 1000;

        unsigned int m_currentUpdateGroup = 0;
        const unsigned int m_maxGroups = 10;

        //Map m_map
};

#endif // SIMULATION_H
