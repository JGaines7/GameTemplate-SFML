#include "SimRenderer.h"
#include <iostream>
#include "ViewportUtilities.h"
#include "appConfig.h"
SimRenderer::SimRenderer()
{
    //ctor
}

SimRenderer::~SimRenderer()
{
    //dtor
}

void SimRenderer::renderSimulation(sf::RenderWindow& window, sf::View& viewport, bool drawAsVertices, Simulation& sim)
{

    sf::Clock clock;
    //draw background (until tilemap);
    drawBg(window, viewport, sim);

    if(sim.getSimulationSettings().drawQuadsRadioInput == 1)
    {
        auto tree = sim.getQuadTree("humans");
        if(tree != nullptr)
        {
            window.draw(*tree);
        }
    }

    if(sim.getSimulationSettings().drawQuadsRadioInput == 2)
    {
        auto tree = sim.getQuadTree("zombies");
        if(tree != nullptr)
        {
            window.draw(*tree);
        }
    }



    drawWorldBounds(window, viewport, sim);


    //cull zombies to viewport. TODO

    sf::CircleShape circle;
    if(sim.getSimulationSettings().highlightZombies)
    {
        circle.setRadius(100);
        circle.setFillColor(sf::Color(255,0,0,55));
        circle.setOrigin(100,100);
    }

    if(!drawAsVertices)
    {
        for (auto& human : sim.getHumans())
        {
            window.draw(*human);
        }

        for (auto& zomb : sim.getZombies())
        {
            if(sim.getSimulationSettings().highlightZombies)
            {
             circle.setPosition(zomb->getPosition());
             window.draw(circle);
            }

            window.draw(*zomb);
        }
    }
    else
    {

    }

    sf::VertexArray pointArray(sf::Points, 0);
    pointArray.setPrimitiveType(sf::Points);

    for( auto& zomb : sim.getZombies())
    {
        if(sim.getSimulationSettings().highlightZombies)
        {
         circle.setPosition(zomb->getPosition());
         window.draw(circle);
        }

        pointArray.append(sf::Vertex(zomb->getPosition(), sf::Color::Red));

    }
    window.draw(pointArray);
    pointArray.clear();
    for( auto& human : sim.getHumans())
    {

        pointArray.append(sf::Vertex(human->getPosition(), sf::Color::Green));

    }
    window.draw(pointArray);


}

void SimRenderer::drawBg(sf::RenderWindow& window, sf::View& viewport, Simulation& sim)
{
    //draw bg
    sf::RectangleShape mapRect(static_cast<sf::Vector2f>(sim.getSimulationSettings().worldDimensions));
    sf::RectangleShape bgRect(viewport.getSize());
    bgRect.setFillColor(sf::Color(35,35,45));
    bgRect.setPosition(ViewportUtil::getViewportTopLeft(viewport));



    //bgRect.setPosition(window.mapPixelToCoords(sf::Vector2i(0, 0)));
    mapRect.setFillColor(sf::Color(15,0,30));
    window.draw(bgRect);
    window.draw(mapRect);
}
void SimRenderer::drawWorldBounds(sf::RenderWindow& window, sf::View& viewport, Simulation& sim)
{

    sf::Vector2i worldSize  = sim.getSimulationSettings().worldDimensions;
    //dont cull to view, probably not worth the calculation overhead.
    sf::Color borderColor = sf::Color::Green;
    sf::VertexArray border(sf::LinesStrip, 0);
    border.append(sf::Vertex(sf::Vector2f(0,0), borderColor));
    border.append(sf::Vertex(sf::Vector2f(0,worldSize.y), borderColor));
    border.append(sf::Vertex(sf::Vector2f(worldSize.x,worldSize.y), borderColor));
    border.append(sf::Vertex(sf::Vector2f(worldSize.x,0), borderColor));
    border.append(sf::Vertex(sf::Vector2f(0,0), borderColor));
    window.draw(border);

}
