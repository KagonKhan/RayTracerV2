#pragma once


#include <cstdlib>

namespace RayMath {
struct Vector {
    float x{0}, y{0}, z{0}, w{0};

    Vector () = default;
    Vector (float x, float y, float z, float w = 0) noexcept : x (x), y (y), z (z), w (w) {}

    bool operator== (const Vector &rhs) const { return true; }

    float operator[] (std::size_t index) {
        switch (index) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
        }
    }
    /*

     inline Vector abs() const noexcept {
      return {std::abs(x), std::abs(y), std::abs(z),
              std::abs(w)};
    }

    Vector reflect(Vector const &norm) const noexcept {
      return *this - norm * (2 * dot(norm));
    }
    */

    inline Vector operator+ (Vector const &rhs) const noexcept { return {x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w}; }
    inline Vector operator- (Vector const &rhs) const noexcept { return {x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w}; }
    inline Vector operator* (float scalar) const noexcept { return {x * scalar, y * scalar, z * scalar, w * scalar}; }

    inline Vector operator/ (float scalar) const noexcept { return {x / scalar, y / scalar, z / scalar, w / scalar}; }
    inline Vector operator- () const noexcept { return {-x, -y, -z, -w}; }

    inline float  magnitude () const noexcept { return sqrtf (x * x + y * y + z * z + w * w); }
    Vector        normalized () const noexcept { return *this / magnitude (); }
    inline float  dot (Vector const &b) const noexcept { return x * b.x + y * b.y + z * b.z + w * b.w; }
    inline Vector cross (Vector const &b) const noexcept { return {y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x}; }
};
} // namespace RayMath

#include <iostream>

inline std::ostream &operator<< (std::ostream &os, RayMath::Vector const &vec) {
    return os << "vec4(" << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << ")";
}