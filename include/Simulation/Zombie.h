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

        void chaseClosestHuman(std::vector<Human>& targets);

    protected:

    private:

};

#endif // ZOMBIE_H
