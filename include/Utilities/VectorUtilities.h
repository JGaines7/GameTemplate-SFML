#ifndef VECTORUTILITIES_H
#define VECTORTUTILITIES_H

#include <SFML/graphics.hpp>
#include <cmath>


namespace VectorUtil
{


    //Consider finding faster less accurate versions for optimization.

    template<typename sfVector>
    float mag(sfVector vec) {return std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2));}

    template<typename sfVector>
    sfVector norm(sfVector vec) { return vec / mag(vec);}

    template<typename sfVector>
    sfVector trunc(sfVector vec, float maxVal)
    {
        float len = VectorUtil::mag(vec);
        if(len > maxVal)
        {
            vec = norm(vec) * maxVal;
        }
        return std::move(vec);
    }

    //Detects intersection between 2 axis aligned rectangles. (Rect has a build in intersect function but may be less efficient since it handles rotateable rects)
    template<typename rect1T, typename rect2T>
    bool parallelRectIntersect(sf::Rect<rect1T>& r1, sf::Rect<rect2T>& r2)
    {
        if((r1.left + r1.width < r2.left) ||
           (r2.left + r2.width < r1.left) ||
           (r1.top + r1.height < r2.top) ||
           (r2.top + r2.height < r1.top))
        {
            return false;
        }
        return true;
    }



};

#endif // VIEWPORTUTILITIES_H
