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
        // Object that contains all links and vertices
        std::shared_ptr<physics::Shape> shape;

        // Body parts of ragdoll
        // They are subsets of shape->links
        std::vector<std::shared_ptr<physics::RigidLink>> rope;
        std::vector<std::shared_ptr<physics::RigidLink>> head;
        std::vector<std::shared_ptr<physics::RigidLink>> leftArm;
        std::vector<std::shared_ptr<physics::RigidLink>> rightArm;
        std::vector<std::shared_ptr<physics::RigidLink>> body;
        std::vector<std::shared_ptr<physics::RigidLink>> legs;

        Player(Game *game, sf::Vector2f position);

        void update(float tdelta) override;
        void draw(sf::RenderWindow &window) const override;
        void moveArms();
        void shootLeftArm();
        void shootRightArm();

    protected:
        void onLinkBroken(std::shared_ptr<physics::RigidLink> link);

        float moveCD;     // Amount of cooldown time for moving (seconds)
        float shootCD;    // Amount of cooldown time for shooting (seconds)
        float leftTimer;  // Timer for left arm shoot cooldown
        float rightTimer; // Timer for right arm shoot cooldown
        float moveTimer;  // Timer for move cooldown

        void ShootBomb(physics::RigidLink &hand);
    };
}