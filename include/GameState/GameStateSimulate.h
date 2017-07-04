#ifndef GAMESTATESIMULATE_H
#define GAMESTATESIMULATE_H

#include <Simulation.h>
#include "GameState/GameState.h"
#include "Zombie.h"
#include "SimRenderer.h"
#include "imgui.h"
#include "imgui-sfml.h"

class GameStateSimulate : public GameState
{
    public:
        GameStateSimulate(Application* app);
        virtual ~GameStateSimulate();

        void handleEvents();
        void update(const float dt);
        void draw(const float dt);

    private:

        sf::View m_mainViewport;
        Simulation m_simulation;
        SimRenderer m_renderer;

        float m_zoomLevel;

        void renderSimulation();
        void renderUi(const float dt);
        void updateWasdMovement(const float dt);

        //private helper functions
        bool handleMouseInputs(sf::Event event);
        bool handleKeyboardInputs(sf::Event event);
        bool handleOther(sf::Event event);
        bool handleFocus(sf::Event event);

        //UI variables until i make class //GUI DEMO TODO

        bool m_paused = false;

        SimulationSettings m_simSettings;


};

#endif // GAMESTATESIMULATE_H
