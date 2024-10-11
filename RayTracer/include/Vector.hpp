#pragma once
#include <array>
#include <compare>

namespace RayMath {

constexpr float constexpr_sqrt(double val) noexcept {
  float curr = val;
  float prev = 0;

  while (curr != prev) {
    prev = std::exchange(curr, 0.5f * (curr + val / curr));
  }

  return curr;
}

struct Vector {
  std::array<float, 4> data{0, 0, 0, 0};
  float &operator[](int index) { return data[index]; }
  float operator[](int index) const { return data[index]; }
  auto operator<=>(Vector const &) const noexcept = default;

  Vector(float x, float y, float z, float w = 0) noexcept
      : data({x, y, z, w}) {}

  Vector operator+(Vector const &rhs) const noexcept {
    return {data[0] + rhs.data[0], data[1] + rhs.data[1], data[2] + rhs.data[2],
            data[3] + rhs.data[3]};
  }
  Vector operator-(Vector const &rhs) const noexcept {
    return {data[0] - rhs.data[0], data[1] - rhs.data[1], data[2] - rhs.data[2],
            data[3] - rhs.data[3]};
  }
  Vector operator-() const noexcept {
    return {-data[0], -data[1], -data[2], -data[3]};
  }

  Vector operator*(float scalar) const noexcept {
    return {data[0] * scalar, data[1] * scalar, data[2] * scalar,
            data[3] * scalar};
  }
  Vector operator/(float scalar) const noexcept {
    return {data[0] / scalar, data[1] / scalar, data[2] / scalar,
            data[3] / scalar};
  }

  Vector abs() const noexcept {
    return {std::abs(data[0]), std::abs(data[1]), std::abs(data[2]),
            std::abs(data[3])};
  }

  float magnitude() const noexcept {
    return constexpr_sqrt(data[0] * data[0] + data[1] * data[1] +
                          data[2] * data[2] + data[3] * data[3]);
  }
  Vector normalized() const noexcept { return *this / magnitude(); }

  auto dot(Vector const &b) const noexcept {
    return data[0] * b[0] + data[1] * b[1] + data[2] * b[2] + data[3] * b[3];
  }
  Vector cross(Vector const &b) const noexcept {
    return {data[1] * b[2] - data[2] * b[1], data[2] * b[0] - data[0] * b[2],
            data[0] * b[1] - data[1] * b[0]};
  }

  Vector reflect(Vector const &norm) const noexcept {
    return *this - norm * (2 * dot(norm));
  }
};

} // namespace RayMath