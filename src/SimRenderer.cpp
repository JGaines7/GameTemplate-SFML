#include "SimRenderer.h"
#include <iostream>
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
    //cull zombies to viewport.

    sf::VertexArray zombPointArray(sf::Points, 0);
    zombPointArray.setPrimitiveType(sf::Points);


    for( auto& zomb : sim.getZombies())
    {
        zombPointArray.append(sf::Vertex(zomb.getPosition(), sf::Color::White));


    }

    window.draw(zombPointArray);


}
