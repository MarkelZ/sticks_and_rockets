#pragma once
#include "physics/shape.hpp"
#include "entity.hpp"
#include <memory>
#include <functional>

namespace game
{
    class Game;

    class Player : public Entity
    {
    public:
        std::shared_ptr<physics::Shape> shape; // make private

        Player(Game *game, sf::Vector2f position);

        void update(float tdelta) override;
        void draw(sf::RenderWindow &window) const override;
        void moveLeftArm();
        void moveRightArm();
        void shoot();

    protected:
        void onLinkBroken(std::shared_ptr<physics::RigidLink> link);

        float moveCD;     // Amount of cooldown time for moving (seconds)
        float shootCD;    // Amount of cooldown time for shooting (seconds)
        float leftTimer;  // Timer for left arm move cooldown
        float rightTimer; // Timer for right arm move cooldown
        float shootTimer; // Timer for shoot cooldown
    };
}