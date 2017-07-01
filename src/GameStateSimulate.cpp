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


        switch(event.type)
        {
            /* Close the window */
            case sf::Event::Closed:
            {
                mainWindow.close();
                break;
            }
            /* Resize the window */  //Maybe we can move common resizing code to base class?  Or should it just be helper functions in util files.
            case sf::Event::Resized:
            {
                m_mainViewport.setSize(event.size.width, event.size.height);
                break;
            }
            case sf::Event::KeyPressed:
            {
                if(event.key.code == sf::Keyboard::Space)
                {
                    m_app->requestStateChange(GameStateEnum::START);
                }
                break;
            }

            default: break;
        }
    }


}
void GameStateSimulate::update(const float dt)
{
    updateWasdMovement(dt);
    auto& win = m_app->getWindow();
    sf::Vector2f pos = win.mapPixelToCoords(sf::Mouse::getPosition(win), m_mainViewport);
    m_simulation.setTargetPos(pos);
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

    //draw bg
    sf::RectangleShape bgRect(sf::Vector2f(mainWindow.getSize()));
    bgRect.setPosition(mainWindow.mapPixelToCoords(sf::Vector2i(0, 0)));
    bgRect.setFillColor(sf::Color(40,0,40));
    mainWindow.draw(bgRect);

    m_renderer.renderSimulation(mainWindow, m_mainViewport, m_simulation);

}

void GameStateSimulate::renderUi(const float dt)
{
    bool pOpen = true;
    //split UI and related variables to their own classes ala TrainManagerUI
    //for now just keep a simple fps renderer in the gamestate object here...
    ImGui::SetNextWindowPos(sf::Vector2f(5,5));
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

    sf::Vector2f viewVelocity;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        viewVelocity.x = -config::KEY_LOOK_SPEED * dt; // pan faster if zoomed out (larger zoom level)
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        viewVelocity.x = config::KEY_LOOK_SPEED * dt;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        viewVelocity.y = -config::KEY_LOOK_SPEED * dt;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        viewVelocity.y = config::KEY_LOOK_SPEED * dt;
    }
    m_mainViewport.move(viewVelocity);
}
