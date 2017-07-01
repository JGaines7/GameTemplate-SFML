#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "GameState.h"

class Application;

enum class GameStateEnum {
        NA, //Used to indicate no desired change
        LOADING_GAME,
        START,
        WORLDGEN,
        SIMULATE
    };

//Simple state manager that takes requests to change state (requestStateChange()) and
//changes the state when it is safe to do so (updateState);

class StateManager
{
    public:
        StateManager(Application* app);
        virtual ~StateManager();

        GameState& getCurrentState();
        void updateState(GameStateEnum state);

    private:
        //member variables
        std::unique_ptr<GameState> m_activeGameState = nullptr;
        Application* m_app; //needed to pass to gamestate constructors.

        //member functions
        std::unique_ptr<GameState> gameStateFromEnum(GameStateEnum state);
        void handleStateChange();
};

#endif // STATEMANAGER_H
