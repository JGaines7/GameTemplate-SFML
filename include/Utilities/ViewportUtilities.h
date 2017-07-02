#ifndef VIEWPORTUTILITIES_H
#define VIEWPORTUTILITIES_H

#include <SFML/graphics.hpp>

namespace ViewportUtil
{
    sf::Vector2f getViewportTopLeft(sf::View& view);
    sf::Vector2f getViewportBottomRight(sf::View& view);

};

#endif // VIEWPORTUTILITIES_H
