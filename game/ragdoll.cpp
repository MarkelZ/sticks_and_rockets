#include "ragdoll.hpp"
#include "particleeffect.hpp"
#include "game.hpp"

namespace game
{
    Ragdoll::Ragdoll(Game *game, sf::Vector2f position)
        : Entity(game), shape(std::make_shared<physics::Shape>("models/ragdoll.toml")),
          age(0.f), maxAge(DEFAULT_MAX_AGE)
    {
        shape->moveTo(position);
        for (auto l : shape->links)
        {
            l->onLinkBroken = std::bind(&Ragdoll::onLinkBroken, this, std::placeholders::_1);
        }
    }

    void Ragdoll::onLinkBroken(std::shared_ptr<physics::RigidLink> link)
    {
        // spawn link broken partiles
        for (int _ = 0; _ < 16; _++)
        {
            auto p = new BreakParticle(game, link->v1.position, link->v2.position);
            game->addEntity(p);
            game->addDynamicObject(p->dynObject);
        }
    }

    void Ragdoll::update(float tdelta)
    {
        age += tdelta;
        if (age >= maxAge)
        {
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

    void Ragdoll::draw(sf::RenderWindow &window) const
    {
        for (auto l : shape->links)
        {
            if (l->isBroken)
                continue;

            sf::Vertex line[] = {sf::Vertex(l->v1.position),
                                 sf::Vertex(l->v2.position)};
            window.draw(line, 2, sf::Lines);
        }
    }
}