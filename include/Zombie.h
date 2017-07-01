#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Entity.h"


class Zombie : public Entity
{
    public:
        Zombie();
        virtual ~Zombie();

        const unsigned int getHealth() const { return m_health;}
        void updatePosition() { setPosition(getPosition() + getVelocity());}

        void setUpdateGroup(unsigned int group) { m_updateGroup = group;}
        const unsigned int getUpdateGroup() const { return m_updateGroup;}



    protected:

    private:

        unsigned int m_health;
        unsigned int m_maxSpeed;
        unsigned int m_updateGroup; //only some zombies will be updated per tick, based on group.
};

#endif // ZOMBIE_H
