#pragma once
#include "vertex.hpp"
#include "rigidlink.hpp"
#include "trigger.hpp"
#include "shape.hpp"
#include <memory>
#include <iostream>

namespace physics
{
    class Simulation
    {
    public: // should be private
        std::vector<std::shared_ptr<DynamicObject>> dynobjects;
        std::vector<std::shared_ptr<Vertex>> vertices;
        std::vector<std::shared_ptr<RigidLink>> links;
        std::vector<std::shared_ptr<Trigger>> triggers;

        float width;
        float height;
        float gravity;

        Simulation(float width, float height, float gravity = 1.0f)
            : width(width), height(height), gravity(gravity) {}

        void update(float tdelta);
        void addShape(Shape &shape);
        void addVertex(std::shared_ptr<Vertex> vertex);
        void addLink(std::shared_ptr<RigidLink> link);
        void addDynamicObject(std::shared_ptr<DynamicObject> dynobj);
        void addTrigger(std::shared_ptr<Trigger> trigger);
        void popVertex(std::shared_ptr<Vertex> vertex);
        void popLink(std::shared_ptr<RigidLink> link);
        void popDynmaicObject(std::shared_ptr<DynamicObject> dynobj);
        void popTrigger(std::shared_ptr<Trigger> trigger);

    private:
        int upd_iters = 2;

        std::vector<std::shared_ptr<DynamicObject>> remDynObjs;
        std::vector<std::shared_ptr<Vertex>> remVertices;
        std::vector<std::shared_ptr<RigidLink>> remLinks;
        std::vector<std::shared_ptr<Trigger>> remTriggers;

        void updateDynamicObjects(float tdelta);
        void updateVertices(float tdelta);
        void updateTriggers(float tdelta);
        void updateDynamicObject(float tdelta, std::shared_ptr<DynamicObject> dobj);
        void updateLinks(float tdelta);
        void constrainVertices(float tdelta);
        void checkTriggers(float tdelta);

        void removeElems();
    };
}