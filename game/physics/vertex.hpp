#pragma once
#include "dynamicobject.hpp"

namespace physics
{
    struct Vertex : DynamicObject
    {
        float elasticity;
        float collideFriction;

        Vertex(sf::Vector2f position, float elasticity = .4f, float collideFriction = 0.9f)
            : DynamicObject(sf::Vector2f(position.x, position.y)),
              elasticity(elasticity), collideFriction(collideFriction) {}

        Vertex(sf::Vector2f position, sf::Vector2f velocity, float elasticity = .4f, float collideFriction = 0.9f)
            : DynamicObject(sf::Vector2f(position.x, position.y), sf::Vector2f(velocity.x, velocity.y)),
              elasticity(elasticity), collideFriction(collideFriction) {}
    };
}