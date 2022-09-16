#include "game.hpp"
#include <cstdlib>
#include <algorithm>

namespace game
{
    Game::Game(int width, int height)
        : width(width), height(height),
          debugwriter("content/freemono.ttf", 24, sf::Color::White),
          input(), simulation(width, height, 0.35f)
    {
        sf::ContextSettings settings;
        settings.antialiasingLevel = 8;

        window = new sf::RenderWindow(
            sf::VideoMode(width, height), "Sticks and Rockets",
            sf::Style::Titlebar | sf::Style::Close, settings);
        window->setFramerateLimit(TPS);

        input.setWindow(window);

        player1 = new Player(this, sf::Vector2f(200.f, 100.f));
        addEntity(player1);
        simulation.addShape(*player1->shape);

        player2 = new Player(this, sf::Vector2f(width - 200.f, 100.f));
        addEntity(player2);
        simulation.addShape(*player2->shape);
    }

    void Game::run()
    {
        while (window->isOpen())
        {
            update();
            draw();
        }
    }

    void Game::addDynamicObject(std::shared_ptr<physics::DynamicObject> dynObj)
    {
        simulation.addDynamicObject(dynObj);
    }

    void Game::addShape(physics::Shape &shape)
    {
        simulation.addShape(shape);
    }

    void Game::addEntity(Entity *entity)
    {
        toAdd.push_back(entity);
    }

    void Game::popEntity(Entity *entity)
    {
        toRemove.push_back(entity);
    }

    void Game::addEntities()
    {
        entities.insert(entities.begin(), toAdd.begin(), toAdd.end());
        toAdd.clear();
    }

    void Game::removeEntities()
    {
        for (auto entity : toRemove)
        {
            auto ind = std::find(entities.begin(), entities.end(), entity);
            if (ind != entities.end())
                entities.erase(ind);
        }
        toRemove.clear();
    }

    int Game::getWidth() { return width; }
    int Game::getHeight() { return height; }

    void Game::update()
    {
        restartTimer();
        input.update();

        sf::Event event;
        while (window->pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::LostFocus:
                std::cout << "Lost focus.\n";
                break;
            default:
                break;
            }
        }

        simulation.update(SPT);

        for (auto e : entities)
        {
            e->update(SPT);
        }

        if (input.actionDown(Input::Action::P1Move))
            player1->moveArms();
        if (input.actionDown(Input::Action::P2Move))
            player2->moveArms();
        if (input.actionPressed(Input::Action::P1Left))
            player1->shootLeftArm();
        if (input.actionPressed(Input::Action::P1Right))
            player1->shootRightArm();
        if (input.actionPressed(Input::Action::P2Left))
            player2->shootLeftArm();
        if (input.actionPressed(Input::Action::P2Right))
            player2->shootRightArm();

        addEntities();
        removeEntities();
    }

    void Game::draw()
    {
        float updateTime = elapsedTime();

        window->clear();

        for (auto e : entities)
        {
            e->draw(*window);
        }

        // Debug text
        float drawTime = elapsedTime() - updateTime;
        debugwriter.clear();
        debugwriter.stream << "P1: 'A' move arms, 'S' shoot left, 'D' shoot right. \n";
        debugwriter.stream << "P2: 'J' move arms, 'K' shoot left, 'L' shoot right. \n";

        // debugwriter.stream << "RigidLinks:   " << simulation.links.size() << "; ";
        // debugwriter.stream << "Vertices:   " << simulation.vertices.size() << "\n";
        // debugwriter.stream << "Entities:   " << entities.size() << "; ";
        // debugwriter.stream << "Dynamic obj.:   " << simulation.dynobjects.size() << "; ";
        // debugwriter.stream << "Triggers:   " << simulation.triggers.size() << "\n";

        debugwriter.stream << "Update: " << std::fixed << std::setprecision(1) << updateTime << " ms; ";
        debugwriter.stream << "Draw:   " << std::fixed << std::setprecision(1) << drawTime << " ms\n";

        debugwriter.draw(*window);

        // This displays window on screen (not part of debug)
        window->display();
    }

    void Game::restartTimer()
    {
        startTime = sc::high_resolution_clock::now();
    }

    float Game::elapsedTime()
    {
        auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
        long long microseconds = sc::duration_cast<std::chrono::microseconds>(elapsed).count();
        float milliseconds = microseconds / 1000.0f;
        return milliseconds;
    }
}
