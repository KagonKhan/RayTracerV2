#pragma once
#include <array>
#include <compare>

namespace RayMath {
struct Vector {
  float x{0}, y{0}, z{0}, w{0};

  Vector() = default;
  inline Vector(float x, float y, float z, float w = 0) noexcept
      : x(x), y(y), z(z), w(w) {}

  constexpr auto operator<=>(Vector const &) const noexcept = default;

  /*

   inline Vector abs() const noexcept {
    return {std::abs(x), std::abs(y), std::abs(z),
            std::abs(w)};
  }

  Vector reflect(Vector const &norm) const noexcept {
    return *this - norm * (2 * dot(norm));
  }
  */

  inline Vector operator+(Vector const &rhs) const noexcept {
    return {x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w};
  }
  inline Vector operator-(Vector const &rhs) const noexcept {
    return {x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w};
  }

  inline Vector operator*(float scalar) const noexcept {
    return {x * scalar, y * scalar, z * scalar, w * scalar};
  }

  inline Vector operator/(float scalar) const noexcept {
    return {x / scalar, y / scalar, z / scalar, w / scalar};
  }

  inline Vector operator-() const noexcept { return {-x, -y, -z, -w}; }

  inline float magnitude() const noexcept {
    return sqrtf(x * x + y * y + z * z + w * w);
  }

  Vector normalized() const noexcept { return *this / magnitude(); }

  inline float dot(Vector const &b) const noexcept {
    return x * b.x + y * b.y + z * b.z + w * b.w;
  }

  inline Vector cross(Vector const &b) const noexcept {
    return {y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x};
  }
};
} // namespace RayMath