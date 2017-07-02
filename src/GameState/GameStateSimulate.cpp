#include "GameStateSimulate.h"
#include "Application.h"
#include "../config/appConfig.h"
#include <stdlib.h>
#include <cassert>

GameStateSimulate::GameStateSimulate(Application* app) : GameState(app)
{
    assert(app != nullptr);

    auto winSize = app->getWindow().getSize();
    std::cout << winSize.x << " : " << winSize.y << "\n";
    m_mainViewport.reset(sf::FloatRect(0, 0, winSize.x, winSize.y));
    m_mainViewport.setCenter(winSize.x / 2, winSize.y / 2);

    m_zoomLevel = 1.0f;

    std::cout << "GamestateSimulate Created!\n";
}

GameStateSimulate::~GameStateSimulate()
{
    std::cout << "GamestateSimulate Destructed!\n";
    //dtor
}

void GameStateSimulate::handleEvents()
{
    sf::Window& mainWindow = m_app->getWindow();

    sf::Event event;

    while(mainWindow.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);


        if(handleFocus(event)) break;
        if(m_app->appHasFocus())
        {
            if(handleMouseInputs(event)) break;
            if(handleKeyboardInputs(event)) break;
        }

        if(handleOther(event)) break;
    }


}
void GameStateSimulate::update(const float dt)
{
    updateWasdMovement(dt);
    auto& win = m_app->getWindow();
    m_simulation.stepSimulation();
}
void GameStateSimulate::draw(const float dt)
{
    //Setup and clear the view
    m_app->getWindow().setView(m_mainViewport);
    m_app->getWindow().clear();
    /******Begin rendering******/
    renderSimulation();

    renderUi(dt);
    /*******End rendering*******/
    m_app->getWindow().display();
}

void GameStateSimulate::renderSimulation()
{
    sf::RenderWindow& mainWindow = m_app->getWindow();
    m_renderer.renderSimulation(mainWindow, m_mainViewport, m_simulation);
}

void GameStateSimulate::renderUi(const float dt)
{
    //split UI and related variables to their own classes ala TrainManagerUI
    //for now just keep a simple fps renderer in the gamestate object here...
    bool pOpen = true; //just says the fps window if open. needed for Begin(); (passing a local variable by reference is scary, but seems to work until i make a better class)
    ImGui::SetNextWindowPos(sf::Vector2f(5,5));

    if (!ImGui::Begin("tips", &pOpen, sf::Vector2f(0,0), 0.3f, ImGuiWindowFlags_NoInputs|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::End();
        return;
    }
    ImGui::Text("WASD to move. Space to reset. Scroll to zoom");
    ImGui::End();

    ImGui::SetNextWindowPos(sf::Vector2f(5,35));
    if (!ImGui::Begin("Counts", &pOpen, sf::Vector2f(0,0), 0.3f, ImGuiWindowFlags_NoInputs|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::End();
        return;
    }

    ImGui::Text("Humans: %d Zombies: %d      ", (int)m_simulation.getHumans().size(), (int)m_simulation.getZombies().size());
    ImGui::End();

    ImGui::SetNextWindowPos(sf::Vector2f(5,65));
    if (!ImGui::Begin("fpsWindow", &pOpen, sf::Vector2f(0,0), 0.3f, ImGuiWindowFlags_NoInputs|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::End();
        return;
    }

    ImGui::Text("FPS: %.0f", 1.0 / dt);
    ImGui::End();



    ImGui::SFML::Render(m_app->getWindow());
}

void GameStateSimulate::updateWasdMovement(const float dt)
{
    if (!m_app->appHasFocus()) return;  //ignore wasd if out of focus

    sf::Vector2f viewVelocity;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        viewVelocity.x = -config::KEY_LOOK_SPEED * dt * m_zoomLevel; // pan faster if zoomed out (larger zoom level)
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        viewVelocity.x = config::KEY_LOOK_SPEED * dt * m_zoomLevel;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        viewVelocity.y = -config::KEY_LOOK_SPEED * dt * m_zoomLevel;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        viewVelocity.y = config::KEY_LOOK_SPEED * dt * m_zoomLevel;
    }
    m_mainViewport.move(viewVelocity);
}

//private helper functions
bool GameStateSimulate::handleMouseInputs(sf::Event event)
{
    bool eventMatch = false;

    switch(event.type)
    {
        /* Zoom the view */
        case sf::Event::MouseWheelMoved:
        {

            if(event.mouseWheel.delta < 0)
            {
                m_zoomLevel += 1;

            }
            else
            {
                m_zoomLevel -= 1;

            }
            if ( m_zoomLevel > config::MIN_ZOOM) m_zoomLevel = config::MIN_ZOOM;
            if ( m_zoomLevel < config::MAX_ZOOM) m_zoomLevel = config::MAX_ZOOM;

            std::cout << m_zoomLevel << "\n";
            sf::Vector2u winSize = m_app->getWindow().getSize();
            sf::Vector2f winSizeF(winSize);
            m_mainViewport.setSize(winSizeF * m_zoomLevel);
            //m_app->getWindow().setView(m_mainViewport);
            eventMatch = true;
            break;
        }
    }

    return eventMatch;
}
bool GameStateSimulate::handleKeyboardInputs(sf::Event event)
{
    bool eventMatch = false;

    switch(event.type)
    {
        case sf::Event::KeyPressed:
        {
            if(event.key.code == sf::Keyboard::Space)
            {
                //m_app->requestStateChange(GameStateEnum::START);
                m_simulation.startSimulation();
                eventMatch = true;
            }
            else if(event.key.code == sf::Keyboard::Escape)
            {
                m_app->getWindow().close();
                eventMatch = true;
            }

            break;
        }
        default: break;
    }

    return eventMatch;
}
bool GameStateSimulate::handleOther(sf::Event event)
{
    bool eventMatch = false;

    switch(event.type)
    {
        case sf::Event::Closed:
        {
            m_app->getWindow().close();
            eventMatch = true;
            break;
        }
        case sf::Event::Resized:
        {
            m_mainViewport.setSize(event.size.width, event.size.height);
            eventMatch = true;
            break;
        }
        default: break;
    }

    return eventMatch;

}
bool GameStateSimulate::handleFocus(sf::Event event)
{
    bool eventMatch = false;

    switch(event.type)
    {
        case sf::Event::LostFocus:
        {
            m_app->setAppHasFocus(false);
            eventMatch = true;
            break;
        }
        case sf::Event::GainedFocus:
        {
            m_app->setAppHasFocus(true);
            eventMatch = true;
            break;
        }
        default: break;
    }
    return eventMatch;
}
