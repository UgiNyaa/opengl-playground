#include <iostream>

#include "colliding.hpp"

glm::bvec3 correct(const Collider& a, glm::vec3& move, const Collider& b)
{
    if (a.type == ColliderType::BOX && b.type == ColliderType::BOX)
        return correct
        (
            static_cast<const Box&>(a), 
            move,
            static_cast<const Box&>(b)
        );
    
    return glm::bvec3(false, false, false);
}

glm::bvec3 correct(const Box& a, glm::vec3& move, const Box& b)
{
    glm::bvec3 intrusion(false, false, false);
    
    auto ma = Box(a.min + move, a.max + move);

    if (!intersects(ma, b))
        return intrusion;
    
    if (ma.min.y < b.max.y && a.min.y > b.max.y)
    {
        intrusion.y = true;
        move.y = b.max.y - a.min.y + 0.001f;
    }

    if (ma.min.x < b.max.x && a.min.x > b.max.x)
    {
        intrusion.x = true;
        move.x = b.max.x - a.min.x + 0.001f;
    }

    if (ma.min.z < b.max.z && a.min.z > b.max.z)
    {
        intrusion.z = true;
        move.z = b.max.z - a.min.z + 0.001f;
    }
    
    return intrusion;
}
