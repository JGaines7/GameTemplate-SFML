#include "Zombie.h"
#include "appConfig.h"
#include "VectorUtilities.h"
#include <iostream>
Zombie::Zombie()
{
    m_health = 100;
    m_maxSpeed = 5;
    m_updateGroup = 0;
    setRadius(4);
    setFillColor(config::zombieColor);
    setOrigin(getRadius(),getRadius());



}

Zombie::~Zombie()
{
    //dtor
}

void Zombie::chaseClosestHuman(std::vector<Human>& targets)
{

    if (targets.size() == 0) return;

    //iterate through targets. chase closest.
    float minDist = 100000000;
    sf::Vector2f finalDirectionVec;

    for(auto& target : targets)
    {
        sf::Vector2f deltaVec = target.getPosition() - getPosition();

        float dist = std::abs(VectorUtil::mag(deltaVec));

        if(dist < minDist)
        {
            minDist = dist;
            finalDirectionVec = deltaVec;
        }
    }

    //modify direction torward hume
    setVelocity(VectorUtil::trunc(getVelocity() + (VectorUtil::norm(finalDirectionVec) * (1.2f / (minDist))), config::zombieMaxSpeed));
}

