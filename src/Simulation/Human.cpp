#include "Human.h"
#include "appConfig.h"
#include "VectorUtilities.h"
Human::Human()
{
    m_health = 10;
    m_maxSpeed = 10;
    m_updateGroup = 0;
    setRadius(4);
    setFillColor(config::humanColor);
    setOrigin(getRadius(),getRadius());
}

Human::~Human()
{
    //dtor
}

void Human::fleeClosestZombie(std::vector<Zombie>& targets)
{

    if (targets.size() == 0) return;

    //iterate through targets. chase closest.
    float minDist = 100000000;
    sf::Vector2f finalDirectionVec;

    for(auto& target : targets)
    {
        sf::Vector2f deltaVec =  getPosition() - target.getPosition();

        float dist = std::abs(VectorUtil::mag(deltaVec));

        if(dist < minDist)
        {
            minDist = dist;
            finalDirectionVec = deltaVec;
        }
    }

    //modify direction torward hume
    setVelocity(VectorUtil::trunc(getVelocity() + (VectorUtil::norm(finalDirectionVec) * (1.0005f / (minDist* 2))), config::humanMaxSpeed));
}
