#pragma once

#include "Material.hpp"
#include "Objects.hpp"

#include <glm/glm.hpp>
#include <vector>



struct Scene {
    std::vector<Sphere>   spheres;
    std::vector<Material> materials;

    void add (Sphere sphere) { spheres.push_back (sphere); }
    void add (Material material) { materials.push_back (material); }
};
