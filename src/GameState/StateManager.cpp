#include "StateManager.h"

#include <cassert>
#include "GameStateStart.h"
#include "GameStateSimulate.h"

StateManager::StateManager(Application* app) : m_app(app)
{
}

StateManager::~StateManager()
{
}

//Processes any requests to change the current state.
void StateManager::updateState(GameStateEnum state)
{
    m_activeGameState = gameStateFromEnum(state);
}

GameState& StateManager::getCurrentState()
{
    assert(m_activeGameState != nullptr);
    GameState& stateRef = *m_activeGameState;  //Create a reference from the ptr (reference safer since it immutable)
    return stateRef;                           //return the reference
}

//creates a new gamestate based on enum.  returns nullptr if NONE or POP state
std::unique_ptr<GameState> StateManager::gameStateFromEnum(GameStateEnum state)
{
    std::unique_ptr<GameState> retVal = nullptr;

    //TODO add remaining states
    switch(state)
    {
    case GameStateEnum::START:
            retVal = std::move(std::make_unique<GameStateStart>(m_app));
            break;
    case GameStateEnum::SIMULATE:
            retVal = std::move(std::make_unique<GameStateSimulate>(m_app));
            break;
    default:
        std::cout << "WARNING: Unhandled Gamestate enum value!\n";
    }

    return retVal;
}
