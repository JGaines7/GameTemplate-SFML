#include "Zombie.h"

Zombie::Zombie() :
    m_health(100),
    m_maxSpeed(5),
    m_updateGroup(0)
{
    //ctor
    setRadius(4);

}

Zombie::~Zombie()
{
    //dtor
}

