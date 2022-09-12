#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "physics/dynamicobject.hpp"

namespace game
{
    class Game;

    class Entity
    {
    public:
        Entity(Game *game) : game(game) {}

        virtual void update(float tdelta) = 0;
        virtual void draw(sf::RenderWindow &window) const = 0;

    protected:
        Game *game;
    };
}