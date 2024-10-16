#pragma once
#include "Vector.hpp"
#include <array>
#include <compare>
namespace RayMath {
struct Point {
  float x{0}, y{0}, z{0}, w{1};
  Point() = default;
  Point(float x, float y, float z, float w = 1) noexcept
      : x(x), y(y), z(z), w(w) {}

  constexpr auto operator<=>(Point const &) const noexcept = default;

  inline Point operator-(Vector const &rhs) const noexcept {
    return Point{x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w};
  }
  inline Vector operator-(Point const &rhs) const noexcept {
    return Vector{x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w};
  }
  inline Point operator+(Vector const &rhs) const noexcept {
    return Point{x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w};
  }
  inline Point operator-() const noexcept { return {-x, -y, -z, -w}; }
};

} // namespace RayMath