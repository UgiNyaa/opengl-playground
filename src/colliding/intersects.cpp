#include "colliding.hpp"

bool intersects(const Collider& a, const Collider& b)
{
    if (a.type == ColliderType::BOX && b.type == ColliderType::BOX)
        return intersects
        (
            static_cast<const Box&>(a), 
            static_cast<const Box&>(b)
        );
    
    return false;
}

bool intersects(const Box& a, const Box& b)
{
    return a.max.x > b.min.x
        && a.max.y > b.min.y
        && a.max.z > b.min.z
        && a.min.x < b.max.x
        && a.min.y < b.max.y
        && a.min.z < b.max.z;
}