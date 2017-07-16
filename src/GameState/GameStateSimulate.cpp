#include "GameStateSimulate.h"
#include "Application.h"
#include "../config/appConfig.h"
#include "ViewportUtilities.h"
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

    //gui demo stuff
    m_simSettings = m_simulation.getSimulationSettings();

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

    //GUI DEMO TODO

    m_simulation.setSimulationSettings(m_simSettings);

    sf::Clock clock;
    if(!m_paused)
    {
        m_simulation.stepSimulation();
    }
    //std::cout << "Step Time: " <<  clock.restart().asSeconds() << "\n";

}
void GameStateSimulate::draw(const float dt)
{
    sf::Clock clock;
    //Setup and clear the view
    m_app->getWindow().setView(m_mainViewport);
    m_app->getWindow().clear();
    /******Begin rendering******/
    renderSimulation();

    renderUi(dt);
    /*******End rendering*******/
    m_app->getWindow().display();

    //std::cout << "DrawTime: " <<  clock.restart().asSeconds() << "\n";
}

void GameStateSimulate::renderSimulation()
{
    sf::RenderWindow& mainWindow = m_app->getWindow();
    bool drawAsVertices = m_zoomLevel >= 4;
    m_renderer.renderSimulation(mainWindow, m_mainViewport, drawAsVertices, m_simulation);
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
    ImGui::Text("WASD or right click to move. Scroll to zoom");
    ImGui::End();

    ImGui::SetNextWindowPos(sf::Vector2f(350,5));
    if (!ImGui::Begin("Counts", &pOpen, sf::Vector2f(0,0), 0.3f, ImGuiWindowFlags_NoInputs|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::End();
        return;
    }

    ImGui::Text("Humans: %d Zombies: %d      ", (int)m_simulation.getHumans().size(), (int)m_simulation.getZombies().size());
    ImGui::End();

    ImGui::SetNextWindowPos(sf::Vector2f(600,5));
    if (!ImGui::Begin("fpsWindow", &pOpen, sf::Vector2f(0,0), 0.3f, ImGuiWindowFlags_NoInputs|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::End();
        return;
    }

    ImGui::Text("FPS: %.0f", 1.0 / dt);
    ImGui::End();

    ImGui::SetNextWindowPos(sf::Vector2f(5,45));
    //ImGui::SetNextWindowSize(sf::Vector2f(240,200));
    ImGuiWindowFlags flags =  ImGuiWindowFlags_AlwaysAutoResize  | ImGuiWindowFlags_NoMove ;

    ImGui::Begin("Simulation Settings", &pOpen, flags); // begin window
    // slider will fill the space and leave 100 pixels for the label
    ImGui::PushItemWidth(100);

    ImGui::SliderFloat("Human Max Speed", &m_simSettings.humanMaxSpeed, 0.5, 15);
    //ImGui::SliderFloat("Human Tracking Falloff", &m_simSettings.humanTrackingFalloff, 0.1, 3);
    ImGui::SliderFloat("Human Acceleration", &m_simSettings.humanAccelaration, 0.1, 10.0);
    ImGui::SliderFloat("Human Randomness", &m_simSettings.humanWanderingFactor, 0.0,.01);

    ImGui::Separator();

    ImGui::SliderFloat("Zombie Max Speed", &m_simSettings.zombieMaxSpeed, 0.5, 15);
    //ImGui::SliderFloat("Zombie Tracking Falloff", &m_simSettings.zombieTrackingFalloff, 0.1, 3);
    ImGui::SliderFloat("Zombie Acceleration", &m_simSettings.zombieAccelaration, 0.1, 2.0);
    ImGui::SliderFloat("Zombie Randomness", &m_simSettings.zombieWanderingFactor, 0.0,.01);

    ImGui::Separator();

    ImGui::LabelText("(Require restart)", "World settings");

    ImGui::SliderInt("# starting zombies", &m_simSettings.numZombies, 1,5000);
    ImGui::SliderInt("# starting humans", &m_simSettings.numHumans, 1,5000);

    ImGui::Separator();

    ImGui::LabelText("","Render settings");

    ImGui::RadioButton("Off", &m_simSettings.drawQuadsRadioInput, 0);
    ImGui::RadioButton("Draw Human QuadTree", &m_simSettings.drawQuadsRadioInput, 1);
    ImGui::RadioButton("Draw Zombie QuadTree", &m_simSettings.drawQuadsRadioInput, 2);

    ImGui::Checkbox("Highlight Zombies", &m_simSettings.highlightZombies);
    ImGui::Separator();

    if (ImGui::Button("Defaults")) { m_simSettings = SimulationSettings(); }
    ImGui::SameLine();
    if (ImGui::Button("Restart")) { m_simulation.startSimulation(); }
    ImGui::SameLine();
    std::string buttonText = "Pause##togglePause";
    if (m_paused)
    {
        buttonText = "Resume##togglePause";
    }
    if (ImGui::Button(buttonText.c_str())) { m_paused = !m_paused; }




    ImGui::End(); // end window


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
                m_zoomLevel += 0.75;

            }
            else
            {
                m_zoomLevel -= 0.75;

            }
            if ( m_zoomLevel > config::MIN_ZOOM) m_zoomLevel = config::MIN_ZOOM;
            if ( m_zoomLevel < config::MAX_ZOOM) m_zoomLevel = config::MAX_ZOOM;

            std::cout << m_zoomLevel << "\n";
            sf::Vector2u winSize = m_app->getWindow().getSize();
            sf::Vector2f winSizeF(winSize);
            m_mainViewport.setSize(winSizeF * m_zoomLevel);
            eventMatch = true;
            break;
        }
        case sf::Event::MouseMoved:
        {
            /* Pan the camera */
            if(m_panning)
            {
                sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(m_app->getWindow()) - m_panningAnchor);
                m_mainViewport.move(-1.0f * pos * m_zoomLevel);
                m_panningAnchor = sf::Mouse::getPosition(m_app->getWindow());

            }
            eventMatch = true;
            break;
        }
        case sf::Event::MouseButtonPressed:
        {
            /* Start panning */
            if(event.mouseButton.button == sf::Mouse::Right && !ImGui::GetIO().WantCaptureMouse)
            {
                if(!m_panning)
                {
                    m_panning = true;
                    m_panningAnchor = sf::Mouse::getPosition(m_app->getWindow());
                }
            }
            if(event.mouseButton.button == sf::Mouse::Left && !ImGui::GetIO().WantCaptureMouse)
            {
                m_simulation.addHuman(m_app->getWindow().mapPixelToCoords(sf::Mouse::getPosition(m_app->getWindow())));
            }
            eventMatch = true;
            break;
        }
        case sf::Event::MouseButtonReleased:
        {
            /* Stop panning */
            if(event.mouseButton.button == sf::Mouse::Right)
            {
                m_panning = false;
            }
            eventMatch = true;
            break;
        }
        default: break;
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
