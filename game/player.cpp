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
            l->maxLength *= 1.5f;

        for (auto l : head)
            l->maxLength *= 1.25f;

        for (auto l : body)
            l->maxLength *= 0.75f;

        for (auto l : leftArm)
            l->maxLength *= 1.15f;

        for (auto l : rightArm)
            l->maxLength *= 1.15f;

        for (auto l : legs)
            l->maxLength *= 0.75f;

        leftArm[2]->maxLength *= 3.f;
        rightArm[2]->maxLength *= 3.f;
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

    void DrawCannon(sf::RenderWindow &window, physics::RigidLink &link, float width, float percentCooldown)
    {
        auto p1 = link.v1.position;
        auto p2 = link.v2.position;
        auto diff = p1 - p2;
        auto perp = vecm::normalized(sf::Vector2f(-diff.y, diff.x));
        auto hwidth = width / 2.f;

        auto r1 = p1 + perp * hwidth;
        auto r2 = p1 - perp * hwidth;
        auto r3 = p2 + perp * hwidth;
        auto r4 = p2 - perp * hwidth;

        sf::ConvexShape rectangle;
        rectangle.setPointCount(4);

        if (percentCooldown == 0.f)
        {
            // Draw empty cannon
            rectangle.setPoint(0, r1);
            rectangle.setPoint(1, r2);
            rectangle.setPoint(2, r4);
            rectangle.setPoint(3, r3);
            rectangle.setFillColor(sf::Color::White);
            window.draw(rectangle);
        }
        else
        {
            // Draw cannon with cooldown
            auto m = p2 + diff * percentCooldown;
            auto m1 = m + perp * hwidth;
            auto m2 = m - perp * hwidth;

            rectangle.setPoint(0, r1);
            rectangle.setPoint(1, r2);
            rectangle.setPoint(2, m2);
            rectangle.setPoint(3, m1);
            rectangle.setFillColor(sf::Color(64, 64, 64, 255));
            window.draw(rectangle);

            rectangle.setPoint(0, m1);
            rectangle.setPoint(1, m2);
            rectangle.setPoint(2, r4);
            rectangle.setPoint(3, r3);
            rectangle.setFillColor(sf::Color::Red);
            window.draw(rectangle);
        }
    }

    void Player::draw(sf::RenderWindow &window) const
    {

        // Draw sticks
        for (auto l : shape->links)
        {
            if (l->isBroken)
                continue;

            sf::Vertex line[] = {sf::Vertex(l->v1.position),
                                 sf::Vertex(l->v2.position)};
            window.draw(line, 2, sf::Lines);
        }

        // Draw hands
        auto leftCD = leftTimer > 0.f ? leftTimer / shootCD : 0.f;
        auto rightCD = rightTimer > 0.f ? rightTimer / shootCD : 0.f;
        DrawCannon(window, *leftArm[2], 24.f, leftCD);
        DrawCannon(window, *rightArm[2], 24.f, rightCD);
    }

    void Player::moveArms()
    {
        if (moveTimer <= 0.f)
        {
            moveTimer = moveCD;

            // leftArm[2]->v1.push(move);
            leftArm[2]->v2.push(sf::Vector2f(-1.f, -20.f));
            // rightArm[2]->v1.push(move);
            rightArm[2]->v2.push(sf::Vector2f(1.f, -20.f));
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
        auto position = hand.v2.position + (Bomb::RADIUS + 1.f) * direction;

        auto bomb = new Bomb(game, position, velocity);
        game->addEntity(bomb);
        game->simulation.addTrigger(bomb->trigger);
    }
}