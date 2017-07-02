#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Entity.h"
#include "Human.h"

class Human;

class Zombie : public Entity
{
    public:
        Zombie();
        virtual ~Zombie();

        void chaseClosestHuman(std::vector<Human>& targets);

    protected:

    private:

};

#endif // ZOMBIE_H
