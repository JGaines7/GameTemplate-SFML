#ifndef HUMAN_H
#define HUMAN_H

#include "Entity.h"
#include "Zombie.h"
#include "SimConfig.h"

class Zombie;

class Human : public Entity
{
    public:
        Human(SimulationSettings* settings);
        virtual ~Human();

        void fleeClosestZombie(std::vector<Zombie>& targets);

    protected:

    private:
};

#endif // HUMAN_H
