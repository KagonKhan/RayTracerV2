#pragma once
#include "Point.hpp"
#include "Vector.hpp"

namespace RayMath {
struct Ray {
    Point  origin;
    Vector direction;

    Ray () = default;
    Ray (Point origin, Vector direction) : origin (origin), direction (direction) {}

    RayMath::Point at (float t) const { return origin + direction * t; }
};

} // namespace RayMath