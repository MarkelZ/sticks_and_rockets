#include "algo.hpp"

namespace game
{
    sf::Color colmul(float x, sf::Color col, bool mulalpha)
    {
        if (mulalpha)
            return sf::Color(x * col.r, x * col.g, x * col.b, x * col.a);
        else
            return sf::Color(x * col.r, x * col.g, x * col.b, col.a);
    }

    float rfloat(float a, float b)
    {
        return a + (b - a) * (rand() / (static_cast<float>(RAND_MAX)));
    }

    sf::Vector2f addBias(sf::Vector2f direction, float dtheta)
    {
        auto theta = std::atan2(direction.y, direction.x);
        theta += rfloat(-dtheta, dtheta);
        return sf::Vector2f(std::cos(theta), std::sin(theta));
    }
}