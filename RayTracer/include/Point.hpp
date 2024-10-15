#pragma once
#include "Vector.hpp"
#include <array>
#include <compare>
namespace RayMath {
struct Point {
    float x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 };
  Point() = default;
  Point(float x, float y, float z, float w = 1) noexcept : x(x), y(y), z(z), w(w) {}

  constexpr auto operator<=>(Point const &) const  noexcept = default;



  /*
  
  Point operator-() const  {
    return {-x, -y, -z, -w};
  }


  Vector operator+(Point const &rhs) const  {
    return Vector{x + rhs.x, y + rhs.y,
                  z + rhs.z, w + rhs.w};
  }

  Point operator+(Vector const &rhs) const  {
    return Point{x + rhs.x, y + rhs.y,
                 z + rhs.z, w + rhs.w};
  }
    Point operator*(float scalar) const  {
    return {x * scalar, y * scalar, z * scalar,
            w * scalar};
  }

  Point operator/(float scalar) const  {
    return {x / scalar, y / scalar, z / scalar,
            w / scalar};
  }

  Point abs() const  {
    return {std::abs(x), std::abs(y), std::abs(z),
            std::abs(w)};
  }

  
  */
  Point operator-(Vector const& rhs) const {
      return Point{ x - rhs.x, y - rhs.y,
                   z - rhs.z, w - rhs.w };
  }
  Vector operator-(Point const& rhs) const {
      return Vector{ x - rhs.x, y - rhs.y,
                    z - rhs.z, w - rhs.w };
  }
};

} // namespace RayMath