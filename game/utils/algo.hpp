#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <SFML/Graphics.hpp>

namespace game
{
    // Multiply the rgba channels of a color col by a scalar x
    sf::Color colmul(float x, sf::Color col, bool mulalpha = true);

    // Return a random float between a and b
    float rfloat(float a = 0.f, float b = 1.f);

    // Given a vector, change angle randomly by at most +-dtheta and normalize
    sf::Vector2f addBias(sf::Vector2f direction, float dtheta);
}