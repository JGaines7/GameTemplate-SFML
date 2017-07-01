#include "Simulation.h"
#include <stdlib.h>
#include <cmath>
#include <iostream>
Simulation::Simulation()
{
    //ctor

        //float dt = elapsed.asSeconds();
    m_zombies.clear();
    m_zombies.reserve(150000);
    for(int i = 0, j = 0; i < 150000; i++ , j =++j % m_maxGroups)
    {

        Zombie zomb;
        zomb.setUpdateGroup(j);
        zomb.setPosition(std::rand() % m_worldWidth, std::rand() % m_worldWidth);
        zomb.setVelocity(sf::Vector2f(std::rand() % 100 * 0.01 - 0.5, (std::rand() % 100 * 0.01) - 0.5));

        m_zombies.push_back(zomb);
    }
}

Simulation::~Simulation()
{
    //dtor
}

void Simulation::setTargetPos(sf::Vector2f pos)
{
    m_target = pos;
}

void Simulation::stepSimulation()
{

    //updateFlowfield (threaded?)
    //updatebsp (different thread?)
    //updateZombieLogic (certain subset of zombies per tick? i.e. keep velocity constant for 5 ticks)
    //moveZombies
    //handleCollisions


    //placeholder zombie movement.
    for( auto& zomb : m_zombies)
    {
        if(zomb.getUpdateGroup() == m_currentUpdateGroup)
        {
            zomb.updateVelocity(sf::Vector2f(std::rand() % 100 - 50.0,std::rand() % 100 - 50.0) * (float)0.005);

            auto vec = m_target - zomb.getPosition();
            auto len = std::sqrt(vec.x * vec.x + vec.y*vec.y);
            auto unitVec = vec / (len*len+1);

            zomb.updateVelocity(unitVec * (float)10.0 );
            zomb.setVelocity(zomb.getVelocity() * (float)0.90);

        }
        zomb.updatePosition();

    }
    m_currentUpdateGroup = (m_currentUpdateGroup + 1) % m_maxGroups;
}
