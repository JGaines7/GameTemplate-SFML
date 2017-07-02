#ifndef GAMESTATESTART_H
#define GAMESTATESTART_H

#include "GameState/GameState.h"

class GameStateStart : public GameState
{
    public:
        GameStateStart(Application* app);
        virtual ~GameStateStart();

        void handleEvents();
        void update(const float dt);
        void draw(const float dt);

    private:
};

#endif // GAMESTATESTART_H
