#pragma once
#include "Vector.hpp"
#include <array>
#include <compare>
namespace RayMath {
struct Point {
  std::array<float, 4> data{0, 0, 0, 1};

  Point(float x, float y, float z, float w = 1) noexcept : data({x, y, z, w}) {}
  float &operator[](int index) { return data[index]; }
  float operator[](int index) const { return data[index]; }
  auto operator<=>(Point const &) const noexcept = default;

  Point operator*(float scalar) const noexcept {
    return {data[0] * scalar, data[1] * scalar, data[2] * scalar,
            data[3] * scalar};
  }

  Point operator/(float scalar) const noexcept {
    return {data[0] / scalar, data[1] / scalar, data[2] / scalar,
            data[3] / scalar};
  }

  Point abs() const noexcept {
    return {std::abs(data[0]), std::abs(data[1]), std::abs(data[2]),
            std::abs(data[3])};
  }

  Point operator-() const noexcept {
    return {-data[0], -data[1], -data[2], -data[3]};
  }
  Vector operator-(Point const &rhs) const noexcept {
    return Vector{data[0] - rhs.data[0], data[1] - rhs.data[1],
                  data[2] - rhs.data[2], data[3] - rhs.data[3]};
  }
  Point operator-(Vector const &rhs) const noexcept {
    return Point{data[0] - rhs.data[0], data[1] - rhs.data[1],
                 data[2] - rhs.data[2], data[3] - rhs.data[3]};
  }
  Vector operator+(Point const &rhs) const noexcept {
    return Vector{data[0] + rhs.data[0], data[1] + rhs.data[1],
                  data[2] + rhs.data[2], data[3] + rhs.data[3]};
  }

  Point operator+(Vector const &rhs) const noexcept {
    return Point{data[0] + rhs.data[0], data[1] + rhs.data[1],
                 data[2] + rhs.data[2], data[3] + rhs.data[3]};
  }
};

} // namespace RayMath