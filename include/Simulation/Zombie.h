#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Entity.h"
#include "Human.h"
#include "SimConfig.h"
class Human;

class Zombie : public Entity
{
    public:
        Zombie(SimulationSettings* settings);
        virtual ~Zombie();

        void chaseClosest(std::vector<Entity*>& targets);

    protected:

    private:

};

#endif // ZOMBIE_H
