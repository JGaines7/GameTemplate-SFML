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
    drawBg(window, viewport);
    drawWorldBounds(window, viewport, sim);

    //cull zombies to viewport.

    for (auto& zomb : sim.getHumans())
    {
        window.draw(zomb);
    }

    for (auto& zomb : sim.getZombies())
    {
        window.draw(zomb);
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

void SimRenderer::drawBg(sf::RenderWindow& window, sf::View& viewport)
{
    //draw bg
    sf::RectangleShape bgRect(sf::Vector2f(viewport.getSize()));
    //bgRect.setPosition(window.mapPixelToCoords(sf::Vector2i(0, 0)));
    bgRect.setFillColor(sf::Color(10,0,10));
    window.draw(bgRect);
}
void SimRenderer::drawWorldBounds(sf::RenderWindow& window, sf::View& viewport, Simulation& sim)
{
    //dont cull to view, probably not worth the calculation overhead.
    sf::Color borderColor = sf::Color::Green;
    sf::VertexArray border(sf::LinesStrip, 0);
    border.append(sf::Vertex(sf::Vector2f(0,0), borderColor));
    border.append(sf::Vertex(sf::Vector2f(0,sim.getWorldSize().y), borderColor));
    border.append(sf::Vertex(sf::Vector2f(sim.getWorldSize().x,sim.getWorldSize().y), borderColor));
    border.append(sf::Vertex(sf::Vector2f(sim.getWorldSize().x,0), borderColor));
    border.append(sf::Vertex(sf::Vector2f(0,0), borderColor));
    window.draw(border);

}
