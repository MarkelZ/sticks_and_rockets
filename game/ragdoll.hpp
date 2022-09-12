#pragma once
#include "physics/shape.hpp"
#include "entity.hpp"
#include <memory>
#include <functional>

namespace game
{
    class Game;

    class Ragdoll : public Entity
    {
    public:
        std::shared_ptr<physics::Shape> shape; // make private

        Ragdoll(Game *game, sf::Vector2f position);

        void update(float tdelta) override;
        void draw(sf::RenderWindow &window) const override;

    private:
        void onLinkBroken(std::shared_ptr<physics::RigidLink> link);
        float age;
        float maxAge;

        static constexpr float DEFAULT_MAX_AGE = 10.f; // despawn after 10 seconds by default
    };
}