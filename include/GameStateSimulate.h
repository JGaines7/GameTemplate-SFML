#ifndef GAMESTATESIMULATE_H
#define GAMESTATESIMULATE_H

#include "Simulation.h"
#include "GameState.h"
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

        void renderSimulation();
        void renderUi(const float dt);
        void updateWasdMovement(const float dt);

};

#endif // GAMESTATESIMULATE_H
