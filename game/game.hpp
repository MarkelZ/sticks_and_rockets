#include <chrono>
#include <iomanip>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils/input.hpp"
#include "utils/debugwriter.hpp"
#include "entity.hpp"
#include "particleeffect.hpp"
#include "physics/simulation.hpp"
#include "ragdoll.hpp"
#include "bomb.hpp"
#include "player.hpp"

namespace game
{

    namespace sc = std::chrono;

    class Game
    {
    private:
        int width;
        int height;

        const int TPS = 60;
        const float SPT = 1.0f / TPS;

        sf::RenderWindow *window;

        void update();
        void draw();

        sc::_V2::system_clock::time_point startTime;
        void restartTimer();
        float elapsedTime();
        void addEntities();
        void removeEntities();

    public:
        Player *player;
        std::vector<Entity *> entities;
        std::vector<Entity *> toAdd;
        std::vector<Entity *> toRemove;

        physics::Simulation simulation;
        Debugwriter debugwriter;
        Input input;

        Game(int width, int height);
        void run();

        void addDynamicObject(std::shared_ptr<physics::DynamicObject> dynObj);
        void addShape(physics::Shape &shape);
        void addEntity(Entity *entity);
        void popEntity(Entity *entity);
        int getWidth();
        int getHeight();
    };
}