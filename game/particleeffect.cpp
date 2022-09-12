#include <cstdlib>
#include "particleeffect.hpp"
#include "game.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

namespace game
{
    // Multiply the rgba channels of a color col by a scalar x
    sf::Color colmul(float x, sf::Color col, bool mulalpha = true)
    {
        if (mulalpha)
            return sf::Color(x * col.r, x * col.g, x * col.b, x * col.a);
        else
            return sf::Color(x * col.r, x * col.g, x * col.b, col.a);
    }

    void ParticleEffect::update(float tdelta)
    {
        // Set circle's origin to dynmaic object's position
        // circle.setOrigin(dynObject->position); // doesn't work
        float radius = circle.getRadius();
        circle.setPosition(dynObject->position.x + radius, dynObject->position.y + radius);

        // Linearly interpolate circle's color based on age
        float agemul = age / maxAge;
        circle.setFillColor(colmul(1.0f - agemul, color1) + colmul(agemul, color2));

        // Age particle and pop from simulation if too old
        age += tdelta;

        if (age >= maxAge)
        {
            game->popEntity(this);
            game->simulation.popDynmaicObject(dynObject);
            delete this;
        }
    }

    void ParticleEffect::draw(sf::RenderWindow &window) const
    {
        if (age >= maxAge)
            return; // we do this for now cuz we dont pop yet

        window.draw(circle);
    }

    sf::Vector2f ParticleEffect::getPosition()
    {
        return dynObject->position;
    }
    void ParticleEffect::setPosition(sf::Vector2f position)
    {
        dynObject->moveTo(position);
    }
    void ParticleEffect::push(sf::Vector2f velocity)
    {
        dynObject->push(velocity);
    }

    // Return a random float between a and b
    float rfloat(float a = 0.f, float b = 1.f)
    {
        return a + (b - a) * (rand() / (static_cast<float>(RAND_MAX)));
    }

    sf::Vector2f addBias(sf::Vector2f direction, float dtheta)
    {
        auto theta = std::atan2(direction.y, direction.x);
        theta += rfloat(-dtheta, dtheta);
        return sf::Vector2f(std::cos(theta), std::sin(theta));
    }

    SparkParticle::SparkParticle(Game *game, sf::Vector2f position, sf::Vector2f direction)
        : ParticleEffect(game, NULL, sf::Color::Black, sf::Color(0, 0, 0, 255), 1.f, rfloat(1.5f, 3.f))
    {
        auto speed = rfloat(5.f, 20.f);
        auto velocity = speed * addBias(direction, 0.4f);
        dynObject = std::make_shared<physics::DynamicObject>(position, velocity);

        float saturation = rfloat(0.5f, 1.f);
        color1 = colmul(saturation, sf::Color(255, 255, 128, 255), false);
        color2 = sf::Color::White;
        dynObject->gravityMul = 0.1f;

        maxAge = 0.25f;

        circle.setPosition(position);
        circle.setFillColor(color1);
    }

    BreakParticle::BreakParticle(Game *game, sf::Vector2f p1, sf::Vector2f p2)
        : ParticleEffect(game, NULL, sf::Color(255, 64, 64, 255), sf::Color(0, 0, 0, 255), 1.f, rfloat(2.f, 4.f))
    {
        // pick random point between p1 and p2
        float alpha = rfloat(0.f, 1.f);
        sf::Vector2f position = p1 * alpha + p2 * (1 - alpha);

        // random direction and speed of initial velocity
        float angle = rfloat(0.f, 360.f); // rfloat(0.f, 2.f * M_PI_2f32);
        sf::Vector2f direction(cos(angle), sin(angle));
        float speed = rfloat(15.f, 20.f);
        sf::Vector2f velocity(speed * cos(angle), speed * sin(angle));

        dynObject = std::make_shared<physics::DynamicObject>(position, velocity);

        float saturation2 = rfloat(0.f, 1.f);
        color2 = colmul(saturation2, color1, false);

        circle.setPosition(position);
        circle.setFillColor(color1);
    }

    ExplosionParticle::ExplosionParticle(Game *game, sf::Vector2f position)
        : ParticleEffect(game, NULL, sf::Color(196, 128, 0, 255), sf::Color(255, 255, 255, 255), 1.f, 64.f)
    {
        maxAge = 0.125f;
        dynObject = std::make_shared<physics::DynamicObject>(
            sf::Vector2f(position.x - 2 * circle.getRadius(), position.y - 2 * circle.getRadius()));
        dynObject->gravityMul = 0.f;
        circle.setPointCount(24);
        circle.setPosition(position);
        circle.setFillColor(color1);
    }

    BoostParticle::BoostParticle(Game *game, sf::Vector2f position)
        : ParticleEffect(game, NULL, sf::Color::Black, sf::Color::Black, 1.f, rfloat(4.f, 8.f))
    {
        auto radius = circle.getRadius();
        sf::Vector2f velocity(rfloat(-10.f, 10.f), rfloat(2.5f, 5.f));
        dynObject = std::make_shared<physics::DynamicObject>(
            sf::Vector2f(position.x - radius, position.y - radius), velocity);

        float saturation = rfloat(0.85f, 1.f);
        color1 = colmul(saturation, sf::Color(0, 255, 0, 255), false);

        maxAge = 0.25f;

        circle.setPosition(position);
        circle.setFillColor(color1);
    }

    SmokeParticle::SmokeParticle(Game *game, sf::Vector2f position)
        : ParticleEffect(game, NULL, sf::Color::White, sf::Color::Black, 1.f, rfloat(4.f, 16.f))
    {
        auto angle = rfloat(0.f, M_PI * 2.f);
        auto speed = rfloat(2.f, 8.f);
        auto velocity = speed * sf::Vector2f(std::cos(angle), std::sin(angle));
        dynObject = std::make_shared<physics::DynamicObject>(position, velocity);
        dynObject->gravityMul = -0.125f;
        dynObject->airFriction = 0.95f;

        float saturation = rfloat(0.85f, 1.f);
        color1 = colmul(saturation, sf::Color(255, 255, 255, 255), false);
        maxAge = 1.5f;

        circle.setPosition(position);
        circle.setFillColor(color1);
    }
}