#include <SFML/Graphics.hpp>
#include <sstream>
#include <string>

namespace game
{
    class Debugwriter
    {
    private:
        sf::Font font;
        sf::Text text;

    public:
        std::stringstream stream;

        Debugwriter(const char fontpath[], int fontsize, sf::Color color);
        void write(std::string str);
        void writeLine(std::string str);
        void clear();
        void draw(sf::RenderWindow &window);
    };
}