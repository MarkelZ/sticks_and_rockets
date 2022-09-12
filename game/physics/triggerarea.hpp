#pragma once
#include <cmath>
#include "vertex.hpp"
#include "rigidlink.hpp"
#include "vectormath.hpp"
#include <iostream>

namespace physics
{
    struct TriggerArea
    {
        virtual bool isTouching(std::shared_ptr<Vertex> vertex) const = 0;
        virtual bool isTouching(std::shared_ptr<RigidLink> link) const = 0;
        virtual void moveTo(sf::Vector2f position) = 0;
    };

    struct CircleArea : TriggerArea
    {
    public:
        CircleArea(sf::Vector2f position, float radius)
            : position(position), radius(radius), radius2(radius * radius) {}

        bool isTouching(std::shared_ptr<Vertex> vertex) const override;
        bool isTouching(std::shared_ptr<RigidLink> link) const override;
        void moveTo(sf::Vector2f position) override;

        sf::Vector2f getPosition();
        void setPosition(sf::Vector2f position);
        float getRadius();
        void setRadius(float radius);
        float getRadius2();

    private:
        sf::Vector2f position;
        float radius;
        float radius2;
    };
}