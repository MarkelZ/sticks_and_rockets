#pragma once
#include "triggerarea.hpp"
#include "dynamicobject.hpp"
#include <functional>

namespace physics
{
    struct Trigger : DynamicObject
    {
        TriggerArea *area;
        bool enabled;
        std::function<void(std::shared_ptr<RigidLink>)> onCollision = [](std::shared_ptr<RigidLink> _) {};

        Trigger(sf::Vector2f position, bool enabled = true)
            : DynamicObject(sf::Vector2f(position.x, position.y)),
              enabled(enabled) {}

        Trigger(sf::Vector2f position, sf::Vector2f velocity, bool enabled = true)
            : DynamicObject(sf::Vector2f(position.x, position.y),
                            sf::Vector2f(velocity.x, velocity.y)),
              enabled(enabled) {}

        ~Trigger()
        {
            if (area)
            {
                delete area;
            }
        }
    };
}