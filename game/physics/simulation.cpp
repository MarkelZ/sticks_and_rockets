#include "simulation.hpp"

namespace physics
{
    void Simulation::addShape(Shape &shape)
    {
        vertices.insert(vertices.end(), shape.vertices.begin(), shape.vertices.end());
        links.insert(links.end(), shape.links.begin(), shape.links.end());
    }

    void Simulation::addVertex(std::shared_ptr<Vertex> vertex)
    {
        vertices.push_back(vertex);
    }

    void Simulation::addLink(std::shared_ptr<RigidLink> link)
    {
        links.push_back(link);
    }

    void Simulation::addDynamicObject(std::shared_ptr<DynamicObject> dynobj)
    {
        dynobjects.push_back(dynobj);
    }

    void Simulation::addTrigger(std::shared_ptr<Trigger> trigger)
    {
        triggers.push_back(trigger);
    }

    void Simulation::popVertex(std::shared_ptr<Vertex> vertex)
    {
        remVertices.push_back(vertex);
    }
    void Simulation::popLink(std::shared_ptr<RigidLink> link)
    {
        remLinks.push_back(link);
    }
    void Simulation::popDynmaicObject(std::shared_ptr<DynamicObject> dynobj)
    {
        remDynObjs.push_back(dynobj);
    }
    void Simulation::popTrigger(std::shared_ptr<Trigger> trigger)
    {
        remTriggers.push_back(trigger);
    }

    template <typename T>
    void removeAndClear(std::vector<T> &vec, std::vector<T> &toRemove)
    {
        for (auto x : toRemove)
        {
            auto ind = std::find(vec.begin(), vec.end(), x);
            if (ind != vec.end())
                vec.erase(ind);
        }
        toRemove.clear();
    }

    void Simulation::removeElems()
    {
        removeAndClear(vertices, remVertices);
        removeAndClear(links, remLinks);
        removeAndClear(dynobjects, remDynObjs);
        removeAndClear(triggers, remTriggers);
    }

    void Simulation::update(float tdelta)
    {
        updateDynamicObjects(tdelta);
        updateVertices(tdelta);
        updateTriggers(tdelta);
        for (int _ = 0; _ < upd_iters; _++)
        {
            updateLinks(tdelta);
            constrainVertices(tdelta);
        }
        checkTriggers(tdelta);

        removeElems();
    }

    void Simulation::updateDynamicObjects(float tdelta)
    {
        for (auto dobj : dynobjects)
        {
            updateDynamicObject(tdelta, dobj);
        }
    }

    void Simulation::updateVertices(float tdelta)
    {
        for (auto vert : vertices)
        {
            updateDynamicObject(tdelta, vert);
        }
    }

    void Simulation::updateTriggers(float tdelta)
    {
        for (auto t : triggers)
        {
            updateDynamicObject(tdelta, t);
            t->area->moveTo(t->position);
        }
    }

    void Simulation::updateDynamicObject(float tdelta, std::shared_ptr<DynamicObject> dobj)
    {
        if (dobj->isFixed)
            return;

        auto diff = dobj->position - dobj->prevpos;
        sf::Vector2f vel(diff.x * dobj->airFriction, diff.y * dobj->airFriction);

        dobj->prevpos.x = dobj->position.x;
        dobj->prevpos.y = dobj->position.y;
        dobj->position += vel;
        dobj->position.y += gravity * dobj->gravityMul;
    }

    void Simulation::updateLinks(float tdelta)
    {
        for (int i = 0; i < links.size(); i++)
        {
            auto l = links[i];

            if (l->isBroken)
                continue;

            auto diff = l->v2.position - l->v1.position;
            auto dist = l->getCurrentLength();

            if (l->canBreak && dist >= l->maxLength)
            {
                l->isBroken = true;
                l->onLinkBroken(l);
                // TODO: pop links[i]
            }

            auto ratio = l->length / (dist + 0.0001f);
            auto hdiff = sf::Vector2f(diff.x / 2, diff.y / 2);
            auto mid = l->v1.position + hdiff;
            auto offset = sf::Vector2f(hdiff.x * ratio, hdiff.y * ratio);

            if (!l->v1.isFixed)
                l->v1.position = mid - offset;
            if (!l->v2.isFixed)
                l->v2.position = mid + offset;
        }
    }

    void Simulation::constrainVertices(float tdelta)
    {
        for (auto v : vertices)
        {
            if (v->position.y > height)
            {
                auto diff = v->position - v->prevpos;
                auto vely = diff.y * v->elasticity;
                v->position.y = height;
                v->prevpos.y = height + vely;
                v->prevpos.x = v->position.x - v->collideFriction * diff.x;
            }
        }
    }

    void Simulation::checkTriggers(float tdelta)
    {
        for (auto t : triggers)
        {
            if (!t->enabled)
                continue;

            for (auto l : links)
            {
                // // TEMP
                // auto v1 = std::make_shared<Vertex>(l->v1.position);
                // auto v2 = std::make_shared<Vertex>(l->v2.position);
                // if (t->area->IsTouching(v1) || t->area->IsTouching(v2))
                // {
                //     t->onCollision(l);
                // }

                if (t->area->isTouching(l))
                {
                    t->onCollision(l);
                }
            }
        }
    }
}