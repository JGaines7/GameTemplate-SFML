//#include "StateStack.h"
//
//#include <cassert>
//
//#include "GameStateStart.h"
//#include "GameStateSimulate.h"
//
//StateStack::StateStack(Application* app, GameStateEnum initialState) : m_app(app)
//{
//    pushState(initialState);
//}
//
//StateStack::~StateStack()
//{
//    //Everything managed by smart pointers. Should handle itself;
//}
//
//GameState& StateStack::getCurrentState()
//{
//    GameState* ptr = m_gameStates.top().get(); //get raw pointer from the unique ptr
//    GameState& stateRef = *ptr;                //Create a reference from the ptr (reference safer since it immutable)
//    return stateRef;                           //return the reference
//}
//
//void StateStack::pushState(GameStateEnum state)
//{
//    auto val = gameStateFromEnum(state);
//    assert(val != nullptr);
//    m_gameStates.push(std::move(val));
//}
//
//
////Processes any requests to change the current state.
//void StateStack::updateState()
//{
//    switch(m_requestedState)
//    {
//    case GameStateEnum::NONE:
//        break; //No change
//    case GameStateEnum::POP:
//        m_gameStates.pop();
//        assert(!m_gameStates.empty());
//        break;
//    default:
//        pushState(m_requestedState); //Rest of enum are new states. Push to stack.
//    }
//    m_requestedState = GameStateEnum::NONE;
//}
//
////creates a new gamestate based on enum.  returns nullptr if NONE or POP state
//std::unique_ptr<GameState> StateStack::gameStateFromEnum(GameStateEnum state)
//{
//    std::unique_ptr<GameState> retVal = nullptr;
//
//    switch(state) //ignores POP and NONE cases
//    {
//    case GameStateEnum::START:
//            retVal = std::move(std::make_unique<GameStateStart>(m_app));
//            break;
//    case GameStateEnum::SIMULATE:
//            retVal = std::move(std::make_unique<GameStateSimulate>(m_app));
//            break;
//    default:
//        std::cout << "Invalid state to create!\n";
//    }
//
//    return retVal;
//
//}
