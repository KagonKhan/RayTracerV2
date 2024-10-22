#pragma once

#include "Objects.hpp"

#include <glm/glm.hpp>
#include <vector>


struct Scene {
    std::vector<Sphere> spheres;

    void add (Sphere sphere) { spheres.push_back (sphere); }
};
