#pragma once
#include "bomb.hpp"
#include "physics/shape.hpp"
#include "physics/trigger.hpp"

namespace game
{
    class Child : public Bomb
    {
    public:
        std::shared_ptr<physics::Shape> shape;

        Child(Game *game, sf::Vector2f position, sf::Vector2f velocity);

        void update(float tdelta) override;
        void draw(sf::RenderWindow &window) const override;

    protected:
        float timer;
        void onLinkBroken(std::shared_ptr<physics::RigidLink> link);
    };
}