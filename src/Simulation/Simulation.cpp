#include "Simulation.h"
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include "VectorUtilities.h"
#include "appConfig.h"
Simulation::Simulation()
{

    //load textures and stuff.
    if(!m_demoTexture.loadFromFile("placeholderSprite.png"))
    {
        std::cout << "Failed to load sprite!\n";
    }

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

        auto human  = std::unique_ptr<Human>(new Human(&m_settings));
        human->setTexture(m_demoTexture);
        human->setColor(sf::Color(0, 255, 0));
        human->setOrigin(5,5);
        human->setUpdateGroup(nextUpdateGroup());
        randomSpawnSettings(*human);

        m_humans.push_back(std::move(human));
    }
    m_zombies.clear();
    m_zombies.reserve(m_settings.numZombies);
    for(int i = 0; i < m_settings.numZombies; i++ )
    {

        auto zombie  = std::unique_ptr<Zombie>(new Zombie(&m_settings));
        zombie->setTexture(m_demoTexture);
        zombie->setColor(sf::Color(255, 0, 0));
        zombie->setOrigin(5,5);
        zombie->setUpdateGroup(nextUpdateGroup());
        randomSpawnSettings(*zombie);
        m_zombies.push_back(std::move(zombie));
    }
}

void Simulation::addHuman(sf::Vector2f position)
{
    auto human  = std::unique_ptr<Human>(new Human(&m_settings));
    human->setTexture(m_demoTexture);
    human->setColor(sf::Color(0, 255, 0));
    human->setOrigin(5,5);
    human->setUpdateGroup(nextUpdateGroup());
    human->setPosition(position);

    m_humans.push_back(std::move(human));
    std::cout << "Adding human at position:" << position.x << " : " << position.y << "\n";
}

void Simulation::stepSimulation()
{

    //updateFlowfield (threaded?)
    //updateQuadTree (different thread?)
    sf::Rect<int> rect(0,0, m_settings.worldDimensions.x,  m_settings.worldDimensions.y);

    //m_quadTrees["humans"].reset(new QuadTree(0, rect));
    m_quadTrees["zombies"].reset(new QuadTree(0, rect));
    m_quadTrees["humans"].reset(new QuadTree(0, rect));

//    for(int i = 0; i < m_humans.size(); i++)
//    {
//        auto& humanTree = m_quadTrees["humans"];
//        humanTree->insert( &m_humans[i]);
//    }

    sf::Clock clock;

    auto& zombieTree = m_quadTrees["zombies"];
    auto& humanTree = m_quadTrees["humans"];

    for(unsigned int i = 0; i < m_zombies.size(); i++)
    {
        zombieTree->insert( m_zombies[i].get());
    }

    for(unsigned int i = 0; i < m_humans.size(); i++)
    {
        humanTree->insert( m_humans[i].get());
    }
    //std::cout << "QuadCreate Time: " <<  clock.restart().asSeconds() << "\n";
    //updateZombieLogic (certain subset of zombies per tick? i.e. keep velocity constant for 5 ticks)
    //moveZombies



    //placeholder zombie movement.
    clock.restart();
    for( auto& zomb : m_zombies)
    {
        if(zomb->getUpdateGroup() == m_currentUpdateGroup)
        {


            zomb->setVelocity(VectorUtil::trunc(zomb->getVelocity(),m_settings.zombieMaxSpeed));

            zomb->updateVelocity(randomDrift(m_settings.zombieWanderingFactor));
            zomb->setVelocity(zomb->getVelocity() * 0.9f);

             sf::Rect<float> rect;
            rect.top = zomb->getPosition().y - 150;
            rect.left = zomb->getPosition().x - 150;
            rect.width = 300;
            rect.height = 300;

            auto preyList = humanTree->retrieve(rect);
            zomb->chaseClosest(preyList);

        }



        zomb->updatePosition();


    }
    //std::cout << "zomb sim Time: " <<  clock.restart().asSeconds() << "\n";
    //placeholder human movement
    for( auto& human : m_humans)
    {
        if(human->getUpdateGroup() == m_currentUpdateGroup)
        {


            human->setVelocity(VectorUtil::trunc(human->getVelocity(),m_settings.humanMaxSpeed));
            human->updateVelocity(randomDrift(m_settings.humanWanderingFactor));
            //human->setVelocity(human->getVelocity() * 0.9f);
            sf::Rect<float> rect;
            rect.top = human->getPosition().y - 150;
            rect.left = human->getPosition().x - 150;
            rect.width = 300;
            rect.height = 300;
            auto threatList = zombieTree->retrieve(rect);
            human->fleeClosest(threatList);
        }

        human->updatePosition();
    }
    //std::cout << "hum sim Time: " <<  clock.restart().asSeconds() << "\n";

    handleCollisions();
    //std::cout << "collision sim Time: " <<  clock.restart().asSeconds() << "\n";

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

    //for each human
    //check collision of human against all existing zombies
    //if collide, kill self and add zombie (to vec and tree)


    for(auto it_human = m_humans.begin(); it_human != m_humans.end();)
    {

        bool gotEaten = false;
        //get vector from tree...


        std::vector<Entity*> localZombies = m_quadTrees["zombies"]->retrieve((*it_human)->getGlobalBounds());


        for(unsigned int i = 0; i < localZombies.size(); i++)//auto it_human_z = localZombies.begin(); it_human_z != localZombies.end(); it_human_z++)
        {

            //TODO: should use rect's collide method after we have rect sprites working
            if(std::abs(VectorUtil::mag((*it_human)->getPosition() - localZombies[i]->getPosition())) < (m_settings.humanRadius + m_settings.zombieRadius))
            {
                //create a new zombie at dead human's position
                auto newZomb  = std::unique_ptr<Zombie>(new Zombie(&m_settings));
                newZomb->setPosition((*it_human)->getPosition());
                newZomb->setUpdateGroup(nextUpdateGroup());
                newZomb->setColor(sf::Color(255, 0, 0));
                newZomb->setTexture(m_demoTexture);
                newZomb->setOrigin(5,5);
                m_zombies.push_back(std::move(newZomb));
                //update zombie quad tree
                m_quadTrees["zombies"]->insert(m_zombies.back().get());

                //swap dying human with human at end of human vector, and then pop it. (removing from inside vector expensive)
                std::swap(*it_human, m_humans.back());
                m_humans.pop_back();

                //TODO: remove human from quad tree if it would make sense to. Right now we dont access it again so safeish.

                //tell outer loop we got eaten and break.
                gotEaten = true;
                break;

            }
        }
        //if we got eaten, then we moved a diff human to same index as last one, so dont increment iterator yet.
        if(!gotEaten)
        {
            it_human++;
        }
    }


}

void Simulation::handleBorderCollisions()
{

    for (auto& zomb : m_zombies)
    {
        handleBorderCollision(*zomb, m_settings.zombieRadius);
    }
    for (auto& human : m_humans)
    {
        handleBorderCollision(*human, m_settings.humanRadius);
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

