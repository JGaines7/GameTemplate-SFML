#ifndef SIMRENDERER_H
#define SIMRENDERER_H

#include <SFML/Graphics.hpp>
#include "Simulation.h"


class SimRenderer
{
    public:
        SimRenderer();
        virtual ~SimRenderer();

        void renderSimulation(sf::RenderWindow& window, sf::View& viewport, bool drawAsVertices, Simulation& sim);

    protected:

    private:
        //avoid trying to keep local references unless we need to.
        void drawBg(sf::RenderWindow& window, sf::View& viewport, Simulation& sim);
        void drawWorldBounds(sf::RenderWindow& window, sf::View& viewport, Simulation& sim);

};

#endif // SIMRENDERER_H
