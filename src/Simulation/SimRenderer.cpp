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

void SimRenderer::renderSimulation(sf::RenderWindow& window, sf::View& viewport, Simulation& sim)
{
    //draw background (until tilemap);
    drawBg(window, viewport, sim);
    drawWorldBounds(window, viewport, sim);

    //cull zombies to viewport.

    sf::CircleShape circle;
    float radius = sim.getSimulationSettings().humanRadius;
    circle.setRadius(radius);
    circle.setFillColor(sim.getSimulationSettings().humanColor);
    circle.setOrigin(radius,radius);

    for (auto& human : sim.getHumans())
    {
        circle.setPosition(human.getPosition());
        window.draw(circle);
    }

    radius = sim.getSimulationSettings().zombieRadius;
    circle.setRadius(radius);
    circle.setFillColor(sim.getSimulationSettings().zombieColor);
    circle.setOrigin(radius,radius);
    for (auto& zomb : sim.getZombies())
    {
        circle.setPosition(zomb.getPosition());
        window.draw(circle);
    }







//Vertex point based drawing. (old)

//    sf::VertexArray zombPointArray(sf::Points, 0);
//    zombPointArray.setPrimitiveType(sf::Points);
//
//    sf::CircleShape renderShape;
//    renderShape.setFillColor(config::zombieColor);
//    renderShape.setRadius()
//
//    for( auto& zomb : sim.getZombies())
//    {
//        zombPointArray.append(sf::Vertex(zomb.getPosition(), sf::Color::White));
//
//    }

    //window.draw(zombPointArray);


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
