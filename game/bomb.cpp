#include <iostream>
#include "bomb.hpp"
#include "game.hpp"

namespace game
{
    Bomb::Bomb(Game *game, sf::Vector2f position, sf::Vector2f velocity)
        : Entity(game), circle(RADIUS, 16), isExploded(false), power(2000.f)
    {
        trigger = std::make_shared<physics::Trigger>(position, velocity);
        trigger->onCollision = std::bind(&Bomb::explode, this);
        trigger->area = new physics::CircleArea(position, RADIUS);

        circle.setFillColor(sf::Color(255, 128, 255, 255));
        circle.setPosition(position);
    }

    void Bomb::update(float tdelta)
    {
        auto radius = circle.getRadius();
        circle.setPosition(trigger->position.x - radius, trigger->position.y - radius);

        if (trigger->position.y + radius >= game->getHeight())
        {
            explode();
        }
    }

    void Bomb::draw(sf::RenderWindow &window) const
    {
        if (isExploded)
            return;

        window.draw(circle);
    }

    void Bomb::explode()
    {
        if (isExploded)
            return;
        isExploded = true;

        // Spawn explosion particle
        auto pos = trigger->position;
        auto explosion = new ExplosionParticle(game, pos);
        game->addEntity(explosion);
        game->addDynamicObject(explosion->dynObject);

        // Sapwn smoke particles
        for (int _ = 0; _ < 48; _++)
        {
            auto particle = new SmokeParticle(game, trigger->position);
            game->addEntity(particle);
            game->simulation.addDynamicObject(particle->dynObject);
        }

        // Push all vertices
        for (auto v : game->simulation.vertices)
        {
            auto diff = v->position - pos;
            auto dist2 = vecm::len2(diff);
            sf::Vector2f push = diff * (power / (1.f + dist2));
            v->push(push);
        }

        // Delete bomb entity
        game->popEntity(this);
        game->simulation.popTrigger(trigger);
    }
}