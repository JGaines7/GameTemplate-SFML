#include "Simulation.h"
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include "VectorUtilities.h"
#include "appConfig.h"
Simulation::Simulation()
{

    startSimulation();

}

Simulation::~Simulation()
{
    //dtor
}

void Simulation::startSimulation()
{

    m_humans.clear();
    m_humans.reserve(m_settings.numHumans);
    for(int i = 0; i < m_settings.numHumans; i++ )
    {

        Human human(&m_settings);
        human.setUpdateGroup(nextUpdateGroup());
        randomSpawnSettings(human);

        m_humans.push_back(std::move(human));
    }
    m_zombies.clear();
    m_zombies.reserve(m_settings.numZombies);
    for(int i = 0; i < m_settings.numZombies; i++ )
    {

        Zombie zombie(&m_settings);
        zombie.setUpdateGroup(nextUpdateGroup());
        randomSpawnSettings(zombie);
        m_zombies.push_back(zombie);
    }
}



void Simulation::stepSimulation()
{

    //updateFlowfield (threaded?)
    //updateQuadTree (different thread?)
    //updateZombieLogic (certain subset of zombies per tick? i.e. keep velocity constant for 5 ticks)
    //moveZombies



    //placeholder zombie movement.
    for( auto& zomb : m_zombies)
    {
        if(zomb.getUpdateGroup() == m_currentUpdateGroup)
        {


            zomb.setVelocity(VectorUtil::trunc(zomb.getVelocity(),m_settings.zombieMaxSpeed));

            zomb.updateVelocity(randomDrift(m_settings.zombieWanderingFactor));
            zomb.setVelocity(zomb.getVelocity() * 0.9f);


        }
        zomb.chaseClosestHuman(m_humans);
        zomb.updatePosition();

    }
    //placeholder human movement
    for( auto& human : m_humans)
    {
        if(human.getUpdateGroup() == m_currentUpdateGroup)
        {


            human.setVelocity(VectorUtil::trunc(human.getVelocity(),m_settings.humanMaxSpeed));
            human.updateVelocity(randomDrift(m_settings.humanWanderingFactor));
            //human.setVelocity(human.getVelocity() * 0.9f);

        }
        human.fleeClosestZombie(m_zombies);
        human.updatePosition();
    }

    handleCollisions();

    nextUpdateGroup();
}


sf::Vector2f Simulation::randomDrift(float factor)
{
    return sf::Vector2f(std::rand() % 1000 - 500.0,std::rand() % 1000 - 500.0) * factor;
}

void Simulation::handleCollisions()
{
    //for every entity, collision check against border. (move if applicable)
    handleBorderCollisions();
    //for every entity, collision check against every other entity

    //if zombie/human collision, kill human, spawn zombie
    checkKills();
    //if z/z or h/h collision, do soft collision bounce.

}

void Simulation::checkKills()
{

    for (int i = 0; i < m_zombies.size(); i++)
    {
        for(auto iter = m_humans.begin(); iter != m_humans.end();)
        {
            if(std::abs(VectorUtil::mag((*iter).getPosition() - m_zombies[i].getPosition())) < (m_settings.humanRadius + m_settings.zombieRadius))
            {
                Zombie newZomb(&m_settings);
                newZomb.setPosition((*iter).getPosition());
                newZomb.setUpdateGroup(nextUpdateGroup());
                std::swap(*iter, m_humans.back());
                m_humans.pop_back();

                m_zombies.push_back(std::move(newZomb));
            }
            else
            {
                iter++;
            }
        }
    }
}

void Simulation::handleBorderCollisions()
{

    for (auto& zomb : m_zombies)
    {
        handleBorderCollision(zomb, m_settings.zombieRadius);
    }
    for (auto& human : m_humans)
    {
        handleBorderCollision(human, m_settings.humanRadius);
    }
}

void Simulation::handleBorderCollision(Entity& ent, float rad)
{

    sf::Vector2f pos = ent.getPosition();

    if(pos.x - rad < 0)
    {
        pos.x = rad;
        ent.setVelocity(0.1f,ent.getVelocity().y);
    }
    if(pos.x > m_settings.worldDimensions.x - rad)
    {
        pos.x = m_settings.worldDimensions.x - rad;
        ent.setVelocity(-0.1f,ent.getVelocity().y);
    }

    if(pos.y - rad < 0)
    {
        pos.y = rad;
        ent.setVelocity(ent.getVelocity().x, 0.1f);
    }
    if(pos.y > m_settings.worldDimensions.y - rad)
    {
        pos.y = m_settings.worldDimensions.y - rad;
        ent.setVelocity(ent.getVelocity().x, -0.1f);
    }

    ent.setPosition(pos);
}

void Simulation::randomSpawnSettings(Entity& ent)
{
    ent.setPosition(std::rand() % m_settings.worldDimensions.x, std::rand() % m_settings.worldDimensions.y);
    ent.setVelocity(sf::Vector2f(std::rand() % 100 * 0.01 - 0.5, (std::rand() % 100 * 0.01) - 0.5));
}
//every tick all entities need to :
//calculate their next move (logic + random)
//updateZombieLogic();
//updateHumanLogic();

