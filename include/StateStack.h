//#ifndef STATESTACK_H
//#define STATESTACK_H
//
//#include <stack>
//#include "GameState.h"
//
//enum class GameStateEnum {
//        NONE, //No state change
//        POP,  //Go back to previous state
//
//        START, //Push new states ...
//        SIMULATE
//    };
//
///*
//    StateStack takes requests for changes to the gamestate stack (i.e. Pop current state or push a new state)
//    The request is processed when the StateStack receives an updateState() call. We assume it is safe to modify the state stack at this point.
//*/
//class StateStack
//{
//    public:
//        StateStack(Application* app, GameStateEnum initialState);
//        virtual ~StateStack();
//
//        GameState& getCurrentState();
//        void requestStateChange(GameStateEnum state) { m_requestedState = state; } //Request application to change game state
//        void updateState(); //Tell StateStack to process a statechange request if any;
//
//    private:
//
//        //member variables
//        std::stack<std::unique_ptr<GameState>> m_gameStates;
//        GameStateEnum m_requestedState = GameStateEnum::NONE;
//        Application* m_app; //needed to pass to gamestate constructors.
//
//        //member functions
//        std::unique_ptr<GameState> gameStateFromEnum(GameStateEnum state);
//        void handleStateChange();
//        void pushState(GameStateEnum state);
//
//};
//
//#endif // STATESTACK_H
