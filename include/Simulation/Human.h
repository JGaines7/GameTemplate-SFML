#ifndef HUMAN_H
#define HUMAN_H

#include "Entity.h"
#include "Zombie.h"

class Zombie;

class Human : public Entity
{
    public:
        Human();
        virtual ~Human();

        void fleeClosestZombie(std::vector<Zombie>& targets);

    protected:

    private:
};

#endif // HUMAN_H
