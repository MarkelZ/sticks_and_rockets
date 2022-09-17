#include <iostream>
#include "bomb.hpp"
#include "game.hpp"
#include "utils/algo.hpp"

namespace game
{
    sf::Vector2f ragdollCenter(physics::Shape &shape)
    {
        return (shape.vertices[6]->position + shape.vertices[7]->position) / 2.f;
    }

    Bomb::Bomb(Game *game, sf::Vector2f position, sf::Vector2f velocity)
        : Entity(game), isExploded(false), power(2500.f), despawnTimer(3.f), enableTimer(0.04f)
    {
        trigger = std::make_shared<physics::Trigger>(position);
        trigger->onCollision = std::bind(&Bomb::explode, this);
        trigger->area = new physics::CircleArea(position, RADIUS);
        trigger->isFixed = true;
        trigger->enabled = false;

        shape = std::make_shared<physics::Shape>("models/ragdoll.toml");
        auto center = ragdollCenter(*shape);
        shape->moveBy(position - center);
        shape->push(velocity);
        // give some spin
        shape->vertices[0]->push(0.5f * velocity);    // head
        shape->vertices[12]->push(-0.25f * velocity); // lhand
        shape->vertices[15]->push(-0.25f * velocity); // rhand
        shape->vertices[18]->push(-0.25f * velocity); // lleg
        shape->vertices[21]->push(-0.25f * velocity); // rleg

        for (auto l : shape->links)
        {
            l->onLinkBroken = std::bind(&Bomb::onLinkBroken, this, std::placeholders::_1);
            l->isCollidable = false;
            l->canBreak = rfloat() < 0.35f; // randomly choose whether a link can break
        }
    }

    void Bomb::update(float tdelta)
    {
        if (enableTimer >= 0.f)
        {
            enableTimer -= tdelta;
            if (enableTimer <= 0.f)
                trigger->enabled = true;
        }

        if (isExploded)
        {
            despawnTimer -= tdelta;
            if (despawnTimer <= 0.f)
            {
                // Despawn
                game->popEntity(this);
                for (auto l : shape->links)
                {
                    game->simulation.popLink(l);
                }
                for (auto v : shape->vertices)
                {
                    game->simulation.popVertex(v);
                }
            }
        }
        else
        {
            // Move trigger to center of ragdoll
            auto center = ragdollCenter(*shape);
            trigger->position = center;
            trigger->area->moveTo(center);
            if (center.y + RADIUS >= game->getHeight())
            {
                explode();
            }
        }
    }

    void Bomb::draw(sf::RenderWindow &window) const
    {
        for (auto l : shape->links)
        {
            if (l->isBroken)
                continue;

            sf::Vertex line[] = {sf::Vertex(l->v1.position, sf::Color::Green),
                                 sf::Vertex(l->v2.position, sf::Color::Green)};
            window.draw(line, 2, sf::Lines);
        }
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

        // Delete trigger
        game->simulation.popTrigger(trigger);
    }

    void Bomb::onLinkBroken(std::shared_ptr<physics::RigidLink> link)
    {
        // Spawn link broken partiles
        for (int _ = 0; _ < 16; _++)
        {
            auto p = new BreakParticle(game, link->v1.position, link->v2.position);
            game->addEntity(p);
            game->addDynamicObject(p->dynObject);
        }
    }
}