#include "Simulation.h"
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include "VectorUtilities.h"
#include "appConfig.h"
Simulation::Simulation() :
    m_entityManager(sf::Rect<int>(0,0,m_settings.worldDimensions.x,m_settings.worldDimensions.y))
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
    m_entityManager.reset(sf::Rect<int>(0,0,m_settings.worldDimensions.x,m_settings.worldDimensions.y));
    for(int i = 0; i < m_settings.numHumans; i++ )
    {

        auto human  = std::unique_ptr<Entity>(new Human(&m_settings));
        human->setTexture(m_demoTexture);
        human->setColor(sf::Color(0, 255, 0));
        human->setOrigin(5,5);
        human->setUpdateGroup(nextUpdateGroup());
        randomSpawnSettings(*human);
        m_entityManager.addEntity(EntityType::ENT_HUMAN, human);
    }
    for(int i = 0; i < m_settings.numZombies; i++ )
    {

        auto zombie  = std::unique_ptr<Entity>(new Zombie(&m_settings));
        zombie->setTexture(m_demoTexture);
        zombie->setColor(sf::Color(255, 0, 0));
        zombie->setOrigin(5,5);
        zombie->setUpdateGroup(nextUpdateGroup());
        randomSpawnSettings(*zombie);
        m_entityManager.addEntity(EntityType::ENT_ZOMBIE, zombie);
    }
}

void Simulation::addHuman(sf::Vector2f position)
{
    auto human  = std::unique_ptr<Entity>(new Human(&m_settings));
    human->setTexture(m_demoTexture);
    human->setColor(sf::Color(0, 255, 0));
    human->setOrigin(5,5);
    human->setUpdateGroup(nextUpdateGroup());
    human->setPosition(position);
    m_entityManager.addEntity(EntityType::ENT_HUMAN, human);
}

void Simulation::stepSimulation()
{
    m_entityManager.rebuildQuadTree();
    moveEntities();
    handleCollisions();
    nextUpdateGroup(); //
}


sf::Vector2f Simulation::randomDrift(float factor)
{
    return sf::Vector2f(std::rand() % 1000 - 500.0,std::rand() % 1000 - 500.0) * factor;
}

void Simulation::handleCollisions()
{
    handleBorderCollisions();
    checkKills();


}

void Simulation::checkKills()
{
    auto humans = m_entityManager.getEntityList(EntityType::ENT_HUMAN);

    for(auto it_human = humans.begin(); it_human != humans.end(); it_human++)
    {
        std::vector<Entity*> localZombies = m_entityManager.getNearbyEntitiesFromQuad(EntityType::ENT_ZOMBIE, (*it_human)->getGlobalBounds());


        for(unsigned int i = 0; i < localZombies.size(); i++)
        {

            //TODO: should use rect's collide method after we have rect sprites working
            if(std::abs(VectorUtil::mag((*it_human)->getPosition() - localZombies[i]->getPosition())) < (m_settings.humanRadius + m_settings.zombieRadius))
            {
                //create a new zombie at dead human's position
                auto newZomb  = std::unique_ptr<Entity>(new Zombie(&m_settings));
                newZomb->setPosition((*it_human)->getPosition());
                newZomb->setUpdateGroup(nextUpdateGroup());
                newZomb->setColor(sf::Color(255, 0, 0));
                newZomb->setTexture(m_demoTexture);
                newZomb->setOrigin(5,5);
                m_entityManager.addEntity(EntityType::ENT_ZOMBIE, newZomb);
                m_entityManager.destroyEntity(EntityType::ENT_HUMAN, (*it_human));
                break;

            }
        }
    }
}

void Simulation::handleBorderCollisions()
{


    for (auto& zomb :  m_entityManager.getEntityList(EntityType::ENT_ZOMBIE))
    {
        handleBorderCollision(*zomb, m_settings.zombieRadius);
    }
    for (auto& human :  m_entityManager.getEntityList(EntityType::ENT_HUMAN))
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

void Simulation::moveEntities()
{

    //placeholder zombie movement.
    for( auto& zomb : m_entityManager.getEntityList(EntityType::ENT_ZOMBIE))
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

            auto preyList = m_entityManager.getNearbyEntitiesFromQuad(EntityType::ENT_HUMAN,rect);

            static_cast<Zombie*>(zomb)->chaseClosest(preyList);

        }
        zomb->updatePosition();
    }
    //placeholder human movement
    for( auto& human : m_entityManager.getEntityList(EntityType::ENT_HUMAN))
    {
        if(human->getUpdateGroup() == m_currentUpdateGroup)
        {


            human->setVelocity(VectorUtil::trunc(human->getVelocity(),m_settings.humanMaxSpeed));
            human->updateVelocity(randomDrift(m_settings.humanWanderingFactor));

            sf::Rect<float> rect;
            rect.top = human->getPosition().y - 150;
            rect.left = human->getPosition().x - 150;
            rect.width = 300;
            rect.height = 300;
            auto threatList =  m_entityManager.getNearbyEntitiesFromQuad(EntityType::ENT_ZOMBIE,rect);
            static_cast<Human*>(human)->fleeClosest(threatList);
        }

        human->updatePosition();
    }
}
//every tick all entities need to :
//calculate their next move (logic + random)
//updateZombieLogic();
//updateHumanLogic();

