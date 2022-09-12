#include "input.hpp"

#include "iostream"

namespace game
{
    constexpr Input::Action ACTIONS[] = {Input::Left, Input::Right, Input::Up, Input::Down, Input::A1, Input::A2};
    constexpr sf::Mouse::Button MOUSE_BUTTONS[] = {sf::Mouse::Button::Left, sf::Mouse::Button::Right, sf::Mouse::Button::Middle, sf::Mouse::Button::XButton1, sf::Mouse::Button::XButton2};

    Input::Input()
    {
        // This should be loaded from a config file
        actionKeys[Action::Left].push_back(sf::Keyboard::Key::Left);
        actionKeys[Action::Left].push_back(sf::Keyboard::Key::A);
        actionKeys[Action::Right].push_back(sf::Keyboard::Key::Right);
        actionKeys[Action::Right].push_back(sf::Keyboard::Key::D);
        actionKeys[Action::Up].push_back(sf::Keyboard::Key::Up);
        actionKeys[Action::Up].push_back(sf::Keyboard::Key::W);
        actionKeys[Action::Down].push_back(sf::Keyboard::Key::Down);
        actionKeys[Action::Down].push_back(sf::Keyboard::Key::S);
        actionKeys[Action::A1].push_back(sf::Keyboard::Key::Space);

        for (Action action : ACTIONS)
        {
            for (auto key : actionKeys[action])
            {
                relevantKeys.push_back(key);
            }
        }
    }

    void Input::setWindow(sf::RenderWindow *window)
    {
        this->window = window;
    }

    // States of the relevant keys are stored in the hashmap
    // This way we have a consistent representation of the keyboard
    void Input::update()
    {
        for (auto key : relevantKeys)
        {
            keyStates[key].second = keyStates[key].first;
            keyStates[key].first = sf::Keyboard::isKeyPressed(key);
        }

        for (auto button : MOUSE_BUTTONS)
        {
            mouseButtonStates[button].second = mouseButtonStates[button].first;
            mouseButtonStates[button].first = sf::Mouse::isButtonPressed(button);
        }
        mousePreviousPosition.x, mousePreviousPosition.y = mouseCurrentPosition.x, mouseCurrentPosition.y;
        mouseCurrentPosition = sf::Mouse::getPosition(*window);
    }

    // A key is down if it is currently being pressed by the user
    bool Input::keyDown(sf::Keyboard::Key key)
    {
        return keyStates[key].first;
    }

    // A key is up if it is not down
    bool Input::keyUp(sf::Keyboard::Key key)
    {
        return !keyDown(key);
    }

    // A key has been pressed iff it is currently down
    // and it was up in the previous game tick
    bool Input::keyPressed(sf::Keyboard::Key key)
    {
        auto keystate = keyStates[key];
        return keystate.first && !keystate.second;
    }

    // A key has been released iff it is currently up
    // and it was down in the previous game tick
    bool Input::keyReleased(sf::Keyboard::Key key)
    {
        auto keystate = keyStates[key];
        return !keystate.first && keystate.second;
    }

    // An action if down if at least one of the keys that
    // correspond to the action is down.
    bool Input::actionDown(Action action)
    {
        for (auto key : actionKeys[action])
        {
            if (keyDown(key))
                return true;
        }
        return false;
    }

    // An action is up if all keys corresponding to the action
    // are up, i. e. the action is not down.
    bool Input::actionUp(Action action)
    {
        return !actionDown(action);
    }

    // An action has been pressed iff for all keys of the action
    // 1. At least one is down
    // 2. None of them were down in the previous game tick
    bool Input::actionPressed(Action action)
    {
        if (!actionDown(action))
            return false;
        for (auto key : actionKeys[action])
        {
            if (keyStates[key].second)
                return false;
        }
        return true;
    }

    // An action has been released iff for all keys of the action
    // 1. None of them are down
    // 2. At least one was down in the previous game tick
    bool Input::actionReleased(Action action)
    {
        if (actionDown(action))
            return false;
        for (auto key : actionKeys[action])
        {
            if (keyStates[key].second)
                return true;
        }
        return false;
    }

    bool Input::mouseButtonDown(sf::Mouse::Button button)
    {
        return mouseButtonStates[button].first;
    }

    bool Input::mouseButtonUp(sf::Mouse::Button button)
    {
        return !mouseButtonUp(button);
    }

    bool Input::mouseButtonPressed(sf::Mouse::Button button)
    {
        auto mstate = mouseButtonStates[button];
        return mstate.first && !mstate.second;
    }

    bool Input::mouseButtonReleased(sf::Mouse::Button button)
    {
        auto mstate = mouseButtonStates[button];
        return !mstate.first && mstate.second;
    }

    sf::Vector2i Input::getMouseCurrentPosition()
    {
        return mouseCurrentPosition;
    }

    sf::Vector2i Input::getMousePreviousPosition()
    {
        return mousePreviousPosition;
    }

    sf::Vector2i Input::mouseMoveVector()
    {
        return mouseCurrentPosition - mousePreviousPosition;
    }
}