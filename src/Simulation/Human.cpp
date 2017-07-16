#include "Human.h"
#include "appConfig.h"
#include "VectorUtilities.h"
Human::Human(SimulationSettings* settings) : Entity(settings)
{
    m_updateGroup = 0;
}

Human::~Human()
{
    //dtor
}

void Human::fleeClosest(std::vector<Entity*>& targets)
{

    if (targets.size() == 0) return;

    //iterate through targets. chase closest.
    float minDist = 100000000;
    sf::Vector2f finalDirectionVec;

    for(auto& target : targets)
    {
        sf::Vector2f deltaVec =  getPosition() - target->getPosition();

        float dist = std::abs(VectorUtil::mag(deltaVec));

        if(dist < minDist)
        {
            minDist = dist;
            finalDirectionVec = deltaVec;
        }
    }
    if(minDist > 300) return;
    //modify direction away from zombler
    setVelocity(VectorUtil::trunc(getVelocity() + (VectorUtil::norm(finalDirectionVec) * (m_activeSimulationSettings->humanAccelaration / (minDist * m_activeSimulationSettings->humanTrackingFalloff))), m_activeSimulationSettings->humanMaxSpeed));
}
