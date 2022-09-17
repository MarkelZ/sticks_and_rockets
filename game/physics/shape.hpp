#pragma once
#include "rigidlink.hpp"
#include "vertex.hpp"
#include <memory>

namespace physics
{
    struct Shape
    {
        std::vector<std::shared_ptr<Vertex>> vertices;
        std::vector<std::shared_ptr<RigidLink>> links;

        Shape(std::vector<std::shared_ptr<Vertex>> vertices,
              std::vector<std::shared_ptr<RigidLink>> links,
              float scale = 1.f);

        Shape(const char path[], float scale = 1.f);

        void push(sf::Vector2f velocity);
        void moveTo(sf::Vector2f position);
        void moveBy(sf::Vector2f direction);
        void scaleBy(float scale);
        sf::Vector2f getPosition();
    };
}