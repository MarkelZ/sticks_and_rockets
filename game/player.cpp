#include "player.hpp"
#include "particleeffect.hpp"
#include "game.hpp"

namespace game
{
    Player::Player(Game *game, sf::Vector2f position)
        : Entity(game), shape(std::make_shared<physics::Shape>("models/ragdoll.toml", 2.f)),
          moveCD(0.25f), shootCD(1.f)
    {
        shape->moveTo(position);

        for (auto l : shape->links)
        {
            l->onLinkBroken = std::bind(&Player::onLinkBroken, this, std::placeholders::_1);
        }

        // Initialize bodyparts
        rope.insert(rope.begin(), shape->links.begin(), shape->links.begin() + 3);
        head.insert(head.begin(), shape->links.begin() + 3, shape->links.begin() + 11);
        body.insert(body.begin(), shape->links.begin() + 11, shape->links.begin() + 26);
        leftArm.insert(leftArm.begin(), shape->links.begin() + 26, shape->links.begin() + 29);
        rightArm.insert(rightArm.begin(), shape->links.begin() + 29, shape->links.begin() + 32);
        legs.insert(legs.begin(), shape->links.begin() + 32, shape->links.end());

        // Fix rope
        shape->vertices[0]->isFixed = true;

        // Strengths of body parts
        for (auto l : rope)
            l->maxLength *= 1.25f;

        for (auto l : head)
            l->maxLength *= 1.25f;

        for (auto l : body)
            l->maxLength *= 1.25f;

        for (auto l : leftArm)
            l->maxLength *= 2.f;

        for (auto l : rightArm)
            l->maxLength *= 2.f;

        for (auto l : legs)
            l->maxLength *= 0.85f;
    }

    void Player::onLinkBroken(std::shared_ptr<physics::RigidLink> link)
    {
        // Spawn link broken partiles
        for (int _ = 0; _ < 16; _++)
        {
            auto p = new BreakParticle(game, link->v1.position, link->v2.position);
            game->addEntity(p);
            game->addDynamicObject(p->dynObject);
        }
    }

    void Player::update(float tdelta)
    {
        // Update timers
        if (leftTimer > 0.f)
            leftTimer -= tdelta;
        if (rightTimer > 0.f)
            rightTimer -= tdelta;
        if (moveTimer > 0.f)
            moveTimer -= tdelta;
    }

    void Player::draw(sf::RenderWindow &window) const
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

    void Player::moveArms(sf::Vector2f move)
    {
        if (moveTimer <= 0.f)
        {
            moveTimer = moveCD;

            leftArm[2]->v1.push(move);
            leftArm[2]->v2.push(move);
            rightArm[2]->v1.push(move);
            rightArm[2]->v2.push(move);
        }
    }

    void Player::shootLeftArm()
    {
        if (leftTimer <= 0.f)
        {
            leftTimer = shootCD;
            ShootBomb(*leftArm[2]);
        }
    }

    void Player::shootRightArm()
    {
        if (rightTimer <= 0.f)
        {
            rightTimer = shootCD;
            ShootBomb(*rightArm[2]);
        }
    }

    void Player::ShootBomb(physics::RigidLink &hand)
    {
        if (hand.isBroken)
            return;

        // velocity
        auto direction = vecm::normalized(hand.v2.position - hand.v1.position);
        auto power = 20.f;
        auto velocity = direction * power;

        // Add some offset to position
        // so that it does not collide with the hand
        auto position = hand.v2.position + 25.f * direction;

        auto bomb = new Bomb(game, position, velocity);
        game->addEntity(bomb);
        game->simulation.addTrigger(bomb->trigger);
    }
}