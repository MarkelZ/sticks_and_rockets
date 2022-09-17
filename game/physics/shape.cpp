#include "shape.hpp"
#include "toml.hpp"

namespace physics
{

    Shape::Shape(std::vector<std::shared_ptr<Vertex>> vertices,
                 std::vector<std::shared_ptr<RigidLink>> links,
                 float scale)
        : vertices(vertices), links(links)
    {
        scaleBy(scale);
    }

    Shape::Shape(const char path[], float scale)
    {
        auto shapetoml = toml::parse_file(path);

        // This is ugly but there is barely any documentation
        // on how to do it properly
        auto vertx = shapetoml["vertices"]["xpos"];
        auto verty = shapetoml["vertices"]["ypos"];
        float xpos, ypos;
        int i = 0;
        while ((xpos = vertx[i].value_or(FLT_MIN)) != FLT_MIN &&
               (ypos = verty[i].value_or(FLT_MIN)) != FLT_MIN)
        {
            auto vertex = std::make_shared<Vertex>(sf::Vector2f(xpos, ypos));
            vertices.push_back(vertex);
            i++;
        }

        auto linkl = shapetoml["links"]["left"];
        auto linkr = shapetoml["links"]["right"];
        int li, ri;
        i = 0;
        while ((li = linkl[i].value_or(-1)) != -1 &&
               (ri = linkr[i].value_or(-1)) != -1)
        {
            auto vl = vertices[li];
            auto vr = vertices[ri];
            auto link = std::make_shared<RigidLink>(*vl, *vr);
            links.push_back(link);
            i++;
        }

        // scale shape
        scaleBy(scale);
    }

    void Shape::push(sf::Vector2f velocity)
    {
        for (auto v : vertices)
        {
            v->push(velocity);
        }
    }

    void Shape::moveTo(sf::Vector2f position)
    {
        if (vertices.empty())
            return;
        moveBy(position - vertices[0]->position);
    }

    void Shape::moveBy(sf::Vector2f direction)
    {
        for (auto v : vertices)
        {
            v->moveBy(direction);
        }
    }

    void Shape::scaleBy(float scale)
    {
        if (vertices.empty())
            return;

        auto origin = vertices[0]->position;
        for (auto v : vertices)
        {
            auto diff = v->position - origin;
            v->moveBy(diff * (scale - 1.f));
        }
        for (int i = 0; i < links.size(); i++)
        {
            links[i]->maxLength *= scale;
            links[i]->length *= scale;
        }
    }

    sf::Vector2f Shape::getPosition()
    {
        if (vertices.empty())
            return sf::Vector2f();
        return vertices[0]->position;
    }
}