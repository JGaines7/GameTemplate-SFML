#ifndef APPLICATION_H
#define APPLICATION_H

#include <GameState.h>
#include "StateManager.h"
#include "ResourceHolder.h"

class StateManager;

class Application
{
    public:
        Application();
        virtual ~Application();

        void requestStateChange(GameStateEnum state) {m_nextState = state;}
        //getters
        sf::RenderWindow& getWindow() { return m_renderWindow;}

        const bool appHasFocus() const { return m_appHasFocus;}
        void setAppHasFocus(bool focus) { m_appHasFocus = focus;} //Gamestates responsible for telling app we lost focus
        //getResourceManager

    private:
        //member variables
        sf::RenderWindow m_renderWindow;
        StateManager m_stateManager;
        //ResourceHolder<sf::Texture, ResourceEnums::Texture> m_textures;
        GameStateEnum m_nextState = GameStateEnum::NA; //Gamestate to push after next tick
        bool m_appHasFocus = true; //Does window have focus (assume true initially)



        void mainLoop();

        //internal helpers
        void updateState();

};

#endif // APPLICATION_H
