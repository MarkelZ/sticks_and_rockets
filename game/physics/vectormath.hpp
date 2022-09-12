#pragma once
#include <SFML/Graphics.hpp>

namespace vecm
{
    using Vector = sf::Vector2f;

    float dot(Vector v1, Vector v2);
    Vector scale(Vector v, float alpha);
    float len(Vector v);
    inline float len2(Vector v) { return dot(v, v); }
    inline float dist(Vector v1, Vector v2) { return len(v2 - v1); }
    inline float dist2(Vector v1, Vector v2) { return len2(v2 - v1); }
}