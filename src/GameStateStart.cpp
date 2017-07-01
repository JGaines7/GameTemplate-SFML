#include "GameStateStart.h"
#include "GameStateSimulate.h"
#include "Application.h"
#include <stdlib.h>
#include <cassert>

GameStateStart::GameStateStart(Application* app) : GameState(app)
{
    assert(app != nullptr);
    std::cout << "GamestateStart Created!\n";

}

GameStateStart::~GameStateStart()
{
    std::cout << "GamestateStart Destructed!\n";
    //dtor
}

void GameStateStart::handleEvents()
{
    sf::Window& mainWindow = m_app->getWindow();

    sf::Event event;

    while(mainWindow.pollEvent(event))
    {
        //ImGui::SFML::ProcessEvent(event);


        switch(event.type)
        {
            /* Close the window */
            case sf::Event::Closed:
            {
                std::cout << "Close event!\n";
                mainWindow.close();
                break;
            }
            /* Resize the window */  //Maybe we can move common resizing code to base class?  Or should it just be helper functions in util files. (prob base class because reference to App)
            case sf::Event::Resized:
            {
                std::cout << "Window resized!";
                //resizeWindow(event.size.width, event.size.height);
                break;
            }
            case sf::Event::KeyPressed:
            {
                std::cout << "Key pressed!\n";
                if(event.key.code == sf::Keyboard::Space)
                {
                    std::cout << "Space!\n";
                    m_app->requestStateChange(GameStateEnum::SIMULATE);
                }
                if(event.key.code == sf::Keyboard::R)
                {
                    std::cout << "Space!\n";
                    m_app->requestStateChange(GameStateEnum::LOADING_GAME);
                }
                break;
            }

            default: break;
        }
    }


}
void GameStateStart::update(const float dt)
{

}
void GameStateStart::draw(const float dt)
{
    sf::RenderWindow& mainWindow = m_app->getWindow();

    mainWindow.clear();
    sf::CircleShape shape(std::rand() % 50 + 50);
    shape.setFillColor(sf::Color::Green);
    mainWindow.draw(shape);
    mainWindow.display();
}
