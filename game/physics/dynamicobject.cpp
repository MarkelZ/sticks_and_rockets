#include "dynamicobject.hpp"

namespace physics
{
    void DynamicObject::push(sf::Vector2f velocity)
    {
        prevpos -= velocity;
    }

    void DynamicObject::moveTo(sf::Vector2f position)
    {
        this->prevpos += position - this->position;
        this->position = position;
    }

    void DynamicObject::moveBy(sf::Vector2f direction)
    {
        position += direction;
        prevpos += direction;
    }
}