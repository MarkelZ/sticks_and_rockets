#pragma once
#include <memory>
#include "entity.hpp"
#include "physics/vertex.hpp"

namespace game
{
    class Player : public Entity
    {
    public:
        Player(Game *game, sf::Vector2f position);
        void update(float tdelta) override;
        void draw(sf::RenderWindow &window) const override;

    protected:
        std::shared_ptr<physics::Vertex> dynobj;
        sf::RectangleShape rect;
    };
}