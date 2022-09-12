#include "rigidlink.hpp"
#include <cmath>

namespace physics
{
    RigidLink::RigidLink(Vertex &v1, Vertex &v2, bool canBreak)
        : v1(v1), v2(v2), length(getCurrentLength()), canBreak(canBreak)
    {
        maxLength = length * MAXLENCOEF;
        isBroken = false;
        onLinkBroken = [](std::shared_ptr<RigidLink> _) {};
    }

    RigidLink::RigidLink(Vertex &v1, Vertex &v2, float length, bool canBreak)
        : v1(v1), v2(v2), length(length), canBreak(canBreak)
    {
        maxLength = length * MAXLENCOEF;
        isBroken = false;
    }

    float RigidLink::getCurrentLength2()
    {
        auto diff = v1.position - v2.position;
        return diff.x * diff.x + diff.y * diff.y;
        // return vecm::dot(diff, diff);
    }

    float RigidLink::getCurrentLength()
    {
        return std::sqrt(getCurrentLength2());
    }
}