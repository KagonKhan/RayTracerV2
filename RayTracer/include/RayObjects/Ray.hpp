#pragma once
#include <glm/glm.hpp>
struct Ray {
    glm::vec3 origin{};
    glm::vec3 direction{};

    Ray () = default;
    Ray (glm::vec3 origin, glm::vec3 direction) : origin (origin), direction (direction) {}

    glm::vec3 at (float t) const { return origin + direction * t; }
};
