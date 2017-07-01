#ifndef GAMESTATE_H
#define GAMESTATE_H

//#include "Application.h"
#include <SFML/Graphics.hpp>
#include <iostream>

class Application; //forward declaration since Application needs Gamestate defined and vice versa. Dependency loop

class GameState
{
    public:
        GameState(Application* app);
        virtual ~GameState();

        virtual void handleEvents() = 0;
        virtual void update(const float dt) = 0;
        virtual void draw(const float dt) = 0;

    protected:
        //pointer to main application. Used to access game resources.
        Application* m_app;

    private:



};

#endif // GAMESTATE_H
