#include "vectormath.hpp"
#include <cmath>

namespace vecm
{
    float dot(Vector v1, Vector v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    Vector scale(Vector v, float alpha)
    {
        return Vector(alpha * v.x, alpha * v.y);
    }

    float len(Vector v)
    {
        return std::sqrt(dot(v, v));
    }
}