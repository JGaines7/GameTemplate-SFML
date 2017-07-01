#include <iostream>
#include <cassert>

#include "Application.h"
#include "GameStateStart.h"
#include "GameStateSimulate.h"
#include "ResourceEnums.h"



Application::Application() : m_stateManager(this)
{
    //Setup window
    std::cout << "Construct renderWindow\n";
    m_renderWindow.create(sf::VideoMode(800, 600), "Swarm Simulator 2: Winter Assault");
    m_renderWindow.setFramerateLimit(60);
    m_renderWindow.setVerticalSyncEnabled(true);

    //setInitialState;
    m_stateManager.updateState(GameStateEnum::SIMULATE);

    ImGui::SFML::Init(m_renderWindow);


    //load textures and stuff.



    //start mainloop
    mainLoop();
}

Application::~Application()
{
    std::cout << "Bye!\n";
}

void Application::mainLoop()
{
    sf::Clock clock;

    while(m_renderWindow.isOpen())
    {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        GameState& currentGameState = m_stateManager.getCurrentState();

        ImGui::SFML::Update(m_renderWindow, elapsed);

        currentGameState.handleEvents();
        currentGameState.update(dt);
        currentGameState.draw(dt);

        //change gamestate if requested
        updateState();


    }
}

void Application::updateState()
{
    if(m_nextState != GameStateEnum::NA)
        {
             m_stateManager.updateState(m_nextState);
             m_nextState = GameStateEnum::NA;
        }
}
