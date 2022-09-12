#pragma once
#include <memory>
#include "entity.hpp"
#include "physics/trigger.hpp"

namespace game
{
    class Bomb : public Entity
    {
    public:
        std::shared_ptr<physics::Trigger> trigger;

        Bomb(Game *game, sf::Vector2f position, sf::Vector2f velocity);

        void update(float tdelta) override;
        void draw(sf::RenderWindow &window) const override;

        void explode();

    protected:
        bool isExploded;
        float power;
        sf::CircleShape circle;
        static constexpr float RADIUS = 16.f;
    };
}