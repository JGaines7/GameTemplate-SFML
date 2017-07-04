#include "Zombie.h"
#include "appConfig.h"
#include "VectorUtilities.h"
#include <iostream>
Zombie::Zombie(SimulationSettings* settings) : Entity(settings)
{
    m_updateGroup = 0;
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
    setVelocity(VectorUtil::trunc(getVelocity() + (VectorUtil::norm(finalDirectionVec) * (m_activeSimulationSettings->zombieAccelaration / (minDist))), m_activeSimulationSettings->zombieMaxSpeed));
}

