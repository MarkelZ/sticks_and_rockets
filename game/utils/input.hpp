#include <SFML/Graphics.hpp>
#include <map>

namespace game
{
    class Input
    {
    public:
        enum Action
        {
            Left,
            Right,
            Up,
            Down,
            A1,
            A2
        };
        static const int hi{1};

        Input();

        void update();

        bool keyDown(sf::Keyboard::Key key);
        bool keyUp(sf::Keyboard::Key key);
        bool keyPressed(sf::Keyboard::Key key);
        bool keyReleased(sf::Keyboard::Key key);

        bool actionDown(Action action);
        bool actionUp(Action action);
        bool actionPressed(Action action);
        bool actionReleased(Action action);

        bool mouseButtonDown(sf::Mouse::Button button);
        bool mouseButtonUp(sf::Mouse::Button button);
        bool mouseButtonPressed(sf::Mouse::Button button);
        bool mouseButtonReleased(sf::Mouse::Button button);
        void setWindow(sf::RenderWindow *window);
        sf::Vector2i getMouseCurrentPosition();
        sf::Vector2i getMousePreviousPosition();
        sf::Vector2i mouseMoveVector();

    private:
        std::map<sf::Keyboard::Key, std::pair<bool, bool>> keyStates;
        std::map<Action, std::vector<sf::Keyboard::Key>> actionKeys;
        std::vector<sf::Keyboard::Key> relevantKeys;
        std::map<sf::Mouse::Button, std::pair<bool, bool>> mouseButtonStates;
        sf::Vector2i mouseCurrentPosition;
        sf::Vector2i mousePreviousPosition;
        sf::RenderWindow *window;

        // static constexpr Input::Action ACTIONS[] = {Action::Left, Action::Right, Action::Up, Action::Down, Action::A1, Action::A2};
    };
}