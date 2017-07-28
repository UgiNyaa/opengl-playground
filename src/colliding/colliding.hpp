#ifndef COLLIDING_HPP
#define COLLIDING_HPP

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/norm.hpp>

enum struct ColliderType
{
    NONE,
    BOX
};

struct Collider
{
    ColliderType type;

    Collider() : type(ColliderType::NONE) { }
    Collider(ColliderType type) : type(type) { }
};

struct Box : public Collider
{
    glm::vec3 min, max;
    
    Box(glm::vec3 min, glm::vec3 max) 
        : Collider(ColliderType::BOX)
        , min(min)
        , max(max) { }
};

bool intersects(const Collider& a, const Collider& b);
bool intersects(const Box& a, const Box& b);

glm::bvec3 correct(const Collider& a, glm::vec3& move, const Collider& b);
glm::bvec3 correct(const Box& a, glm::vec3& move, const Box& b);

#endif /* COLLIDING_HPP */