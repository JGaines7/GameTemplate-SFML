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

void Zombie::chaseClosest(std::vector<Entity*>& targets)
{

    if (targets.size() == 0) return;

    //iterate through targets. chase closest.
    float minDist = 100000000;
    sf::Vector2f finalDirectionVec;

    for(auto& target : targets)
    {
        sf::Vector2f deltaVec = target->getPosition() - getPosition();
        float dist = std::abs(VectorUtil::mag(deltaVec));

        if(dist < minDist)
        {
            minDist = dist;
            finalDirectionVec = deltaVec;

        }
    }

    if(minDist > 300) return;
    auto val = VectorUtil::trunc(this->getVelocity() + (VectorUtil::norm(finalDirectionVec) * (m_activeSimulationSettings->zombieAccelaration)), m_activeSimulationSettings->zombieMaxSpeed);
    //modify direction torward hume
    setVelocity(val);
}

