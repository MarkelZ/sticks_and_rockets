#include "child.hpp"
#include "game.hpp"

namespace game
{
    Child::Child(Game *game, sf::Vector2f position, sf::Vector2f velocity)
        : Bomb(game, position, sf::Vector2f(0.f, 0.f)), timer(3.f)
    {
        trigger->isFixed = true;
        shape = std::make_shared<physics::Shape>("models/ragdoll.toml", 0.5f);
        for (auto l : shape->links)
        {
            l->onLinkBroken = std::bind(&Child::onLinkBroken, this, std::placeholders::_1);
            l->isCollidable = false;
        }
        shape->moveTo(position);
        shape->push(velocity);
        shape->vertices[0]->push(0.5f * velocity); // give some spin
    }

    void Child::update(float tdelta)
    {
        if (isExploded)
        {
            timer -= tdelta;
            if (timer <= 0.f)
            {
                // despawn
            }
        }
        else
        {
            auto center = (shape->vertices[4]->position + shape->vertices[5]->position +
                           shape->vertices[6]->position + shape->vertices[7]->position) /
                          4.f;
            trigger->position = center;
            trigger->area->moveTo(center);
            if (center.y + RADIUS + 16.f >= game->getHeight())
            {
                explode();
            }
        }
    }

    void Child::draw(sf::RenderWindow &window) const
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

    void Child::onLinkBroken(std::shared_ptr<physics::RigidLink> link)
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