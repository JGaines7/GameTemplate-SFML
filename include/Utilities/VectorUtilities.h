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


};

#endif // VIEWPORTUTILITIES_H
