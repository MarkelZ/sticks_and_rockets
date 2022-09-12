#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "physics/dynamicobject.hpp"
#include "entity.hpp"

namespace game
{
    class Game;

    class ParticleEffect : public Entity
    {
    protected:
        sf::Color color1;
        sf::Color color2;
        sf::CircleShape circle;
        float age;
        float maxAge; // lifespan

    public:
        std::shared_ptr<physics::DynamicObject> dynObject; // should be private

        ParticleEffect(Game *game, std::shared_ptr<physics::DynamicObject> dynObject,
                       sf::Color color1, sf::Color color2, float maxAge, float radius)
            : Entity(game), dynObject(dynObject), color1(color1), color2(color2), age(0.f),
              maxAge(maxAge), circle(radius, 16) {}

        void update(float tdelta) override;
        void draw(sf::RenderWindow &window) const override;

        sf::Vector2f getPosition();
        void setPosition(sf::Vector2f position);
        void push(sf::Vector2f velocity);
    };

    class SparkParticle : public ParticleEffect
    {
    public:
        SparkParticle(Game *game, sf::Vector2f position, sf::Vector2f direction);
    };

    class BreakParticle : public ParticleEffect
    {
    public:
        BreakParticle(Game *game, sf::Vector2f p1, sf::Vector2f p2);
    };

    class ExplosionParticle : public ParticleEffect
    {
    public:
        ExplosionParticle(Game *game, sf::Vector2f position);
    };

    class BoostParticle : public ParticleEffect
    {
    public:
        BoostParticle(Game *game, sf::Vector2f position);
    };

    class SmokeParticle : public ParticleEffect
    {
    public:
        SmokeParticle(Game *game, sf::Vector2f position);
    };
}