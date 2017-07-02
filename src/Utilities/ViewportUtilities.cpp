#include "ViewportUtilities.h"

namespace ViewportUtil
{
    sf::Vector2f getViewportTopLeft(sf::View& view)
    {
        auto& viewCenter = view.getCenter();
        auto offset = view.getSize() * 0.5f;

        return viewCenter - offset;
    }
    sf::Vector2f getViewportBottomRight(sf::View& view)
    {
        auto& viewCenter = view.getCenter();
        auto offset = view.getSize() * 0.5f;

        return viewCenter + offset;
    }

}

