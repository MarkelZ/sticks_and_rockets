#include "debugwriter.hpp"

namespace game
{
    Debugwriter::Debugwriter(const char fontpath[], int fontsize, sf::Color color)
    {
        font.loadFromFile(fontpath);
        text.setFont(font);
        text.setCharacterSize(fontsize);
        text.setFillColor(color);
        text.setStyle(sf::Text::Bold);
    }

    void Debugwriter::write(std::string str)
    {
        stream << str;
    }

    void Debugwriter::writeLine(std::string str)
    {
        stream << str << "\n";
    }

    void Debugwriter::clear()
    {
        stream.str(std::string());
    }

    void Debugwriter::draw(sf::RenderWindow &window)
    {
        text.setString(stream.str());
        window.draw(text);
    }
}