#pragma once
#include <memory>
#include "entity.hpp"
#include "physics/trigger.hpp"
#include "physics/shape.hpp"

namespace game
{
    class Bomb : public Entity
    {
    public:
        std::shared_ptr<physics::Trigger> trigger;
        std::shared_ptr<physics::Shape> shape;

        Bomb(Game *game, sf::Vector2f position, sf::Vector2f velocity);

        void update(float tdelta) override;
        void draw(sf::RenderWindow &window) const override;

        void explode();

        static constexpr float RADIUS = 32.f;

    protected:
        bool isExploded;
        float power;
        float despawnTimer;
        float enableTimer;

        void onLinkBroken(std::shared_ptr<physics::RigidLink> link);
    };
}