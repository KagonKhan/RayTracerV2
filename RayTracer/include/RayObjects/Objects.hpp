#pragma once
#include <glm/glm.hpp>

struct Sphere {
    glm::vec3 position{0.0f};
    float     radius{1.0f};

    int materialIndex{0};
};
