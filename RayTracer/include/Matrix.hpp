
#pragma once
#include "Point.hpp"
#include <array>
namespace RayMath {

class Matrix {
public:
  inline const static float eps{0.00001f};

  std::array<float, 4 * 4> data{};
  template <typename... Ts>
  Matrix(Ts &&...ts) : data{std::forward<Ts>(ts)...} {}

  bool operator==(const Matrix &rhs) const noexcept {
    for (int i = 0; i < 16; i++)
      if (std::fabsf(rhs.data[i] - data[i]) > eps)
        return false;

    return true;
  }

  // TODO: Convert to multi-dim sub operator when available
  inline static constexpr std::size_t mapIndex(const std::size_t row,
                                               const std::size_t col) noexcept {
    return 4 * row + col;
  }

  Matrix operator*(Matrix const &rhs) const {
    float v1 = data[mapIndex(0, 0)] * rhs.data[mapIndex(0, 0)] +
               data[mapIndex(0, 1)] * rhs.data[mapIndex(1, 0)] +
               data[mapIndex(0, 2)] * rhs.data[mapIndex(2, 0)] +
               data[mapIndex(0, 3)] * rhs.data[mapIndex(3, 0)];
    float v2 = data[mapIndex(0, 0)] * rhs.data[mapIndex(0, 1)] +
               data[mapIndex(0, 1)] * rhs.data[mapIndex(1, 1)] +
               data[mapIndex(0, 2)] * rhs.data[mapIndex(2, 1)] +
               data[mapIndex(0, 3)] * rhs.data[mapIndex(3, 1)];
    float v3 = data[mapIndex(0, 0)] * rhs.data[mapIndex(0, 2)] +
               data[mapIndex(0, 1)] * rhs.data[mapIndex(1, 2)] +
               data[mapIndex(0, 2)] * rhs.data[mapIndex(2, 2)] +
               data[mapIndex(0, 3)] * rhs.data[mapIndex(3, 2)];
    float v4 = data[mapIndex(0, 0)] * rhs.data[mapIndex(0, 3)] +
               data[mapIndex(0, 1)] * rhs.data[mapIndex(1, 3)] +
               data[mapIndex(0, 2)] * rhs.data[mapIndex(2, 3)] +
               data[mapIndex(0, 3)] * rhs.data[mapIndex(3, 3)];

    float v5 = data[mapIndex(1, 0)] * rhs.data[mapIndex(0, 0)] +
               data[mapIndex(1, 1)] * rhs.data[mapIndex(1, 0)] +
               data[mapIndex(1, 2)] * rhs.data[mapIndex(2, 0)] +
               data[mapIndex(1, 3)] * rhs.data[mapIndex(3, 0)];
    float v6 = data[mapIndex(1, 0)] * rhs.data[mapIndex(0, 1)] +
               data[mapIndex(1, 1)] * rhs.data[mapIndex(1, 1)] +
               data[mapIndex(1, 2)] * rhs.data[mapIndex(2, 1)] +
               data[mapIndex(1, 3)] * rhs.data[mapIndex(3, 1)];
    float v7 = data[mapIndex(1, 0)] * rhs.data[mapIndex(0, 2)] +
               data[mapIndex(1, 1)] * rhs.data[mapIndex(1, 2)] +
               data[mapIndex(1, 2)] * rhs.data[mapIndex(2, 2)] +
               data[mapIndex(1, 3)] * rhs.data[mapIndex(3, 2)];
    float v8 = data[mapIndex(1, 0)] * rhs.data[mapIndex(0, 3)] +
               data[mapIndex(1, 1)] * rhs.data[mapIndex(1, 3)] +
               data[mapIndex(1, 2)] * rhs.data[mapIndex(2, 3)] +
               data[mapIndex(1, 3)] * rhs.data[mapIndex(3, 3)];

    float v9 = data[mapIndex(2, 0)] * rhs.data[mapIndex(0, 0)] +
               data[mapIndex(2, 1)] * rhs.data[mapIndex(1, 0)] +
               data[mapIndex(2, 2)] * rhs.data[mapIndex(2, 0)] +
               data[mapIndex(2, 3)] * rhs.data[mapIndex(3, 0)];
    float v10 = data[mapIndex(2, 0)] * rhs.data[mapIndex(0, 1)] +
                data[mapIndex(2, 1)] * rhs.data[mapIndex(1, 1)] +
                data[mapIndex(2, 2)] * rhs.data[mapIndex(2, 1)] +
                data[mapIndex(2, 3)] * rhs.data[mapIndex(3, 1)];
    float v11 = data[mapIndex(2, 0)] * rhs.data[mapIndex(0, 2)] +
                data[mapIndex(2, 1)] * rhs.data[mapIndex(1, 2)] +
                data[mapIndex(2, 2)] * rhs.data[mapIndex(2, 2)] +
                data[mapIndex(2, 3)] * rhs.data[mapIndex(3, 2)];
    float v12 = data[mapIndex(2, 0)] * rhs.data[mapIndex(0, 3)] +
                data[mapIndex(2, 1)] * rhs.data[mapIndex(1, 3)] +
                data[mapIndex(2, 2)] * rhs.data[mapIndex(2, 3)] +
                data[mapIndex(2, 3)] * rhs.data[mapIndex(3, 3)];

    float v13 = data[mapIndex(3, 0)] * rhs.data[mapIndex(0, 0)] +
                data[mapIndex(3, 1)] * rhs.data[mapIndex(1, 0)] +
                data[mapIndex(3, 2)] * rhs.data[mapIndex(2, 0)] +
                data[mapIndex(3, 3)] * rhs.data[mapIndex(3, 0)];
    float v14 = data[mapIndex(3, 0)] * rhs.data[mapIndex(0, 1)] +
                data[mapIndex(3, 1)] * rhs.data[mapIndex(1, 1)] +
                data[mapIndex(3, 2)] * rhs.data[mapIndex(2, 1)] +
                data[mapIndex(3, 3)] * rhs.data[mapIndex(3, 1)];
    float v15 = data[mapIndex(3, 0)] * rhs.data[mapIndex(0, 2)] +
                data[mapIndex(3, 1)] * rhs.data[mapIndex(1, 2)] +
                data[mapIndex(3, 2)] * rhs.data[mapIndex(2, 2)] +
                data[mapIndex(3, 3)] * rhs.data[mapIndex(3, 2)];
    float v16 = data[mapIndex(3, 0)] * rhs.data[mapIndex(0, 3)] +
                data[mapIndex(3, 1)] * rhs.data[mapIndex(1, 3)] +
                data[mapIndex(3, 2)] * rhs.data[mapIndex(2, 3)] +
                data[mapIndex(3, 3)] * rhs.data[mapIndex(3, 3)];

    return {v1, v2,  v3,  v4,  v5,  v6,  v7,  v8,
            v9, v10, v11, v12, v13, v14, v15, v16};
  }

  Vector operator*(Vector const &rhs) const {
    float v1 = data[mapIndex(0, 0)] * rhs.x + data[mapIndex(0, 1)] * rhs.y +
               data[mapIndex(0, 2)] * rhs.z + data[mapIndex(0, 3)] * rhs.w;
    float v2 = data[mapIndex(1, 0)] * rhs.x + data[mapIndex(1, 1)] * rhs.y +
               data[mapIndex(1, 2)] * rhs.z + data[mapIndex(1, 3)] * rhs.w;
    float v3 = data[mapIndex(2, 0)] * rhs.x + data[mapIndex(2, 1)] * rhs.y +
               data[mapIndex(2, 2)] * rhs.z + data[mapIndex(2, 3)] * rhs.w;
    float v4 = data[mapIndex(3, 0)] * rhs.x + data[mapIndex(3, 1)] * rhs.y +
               data[mapIndex(3, 2)] * rhs.z + data[mapIndex(3, 3)] * rhs.w;

    return {v1, v2, v3, v4};
  }

  Point operator*(Point const &rhs) const {
    float v1 = data[mapIndex(0, 0)] * rhs.x + data[mapIndex(0, 1)] * rhs.y +
               data[mapIndex(0, 2)] * rhs.z + data[mapIndex(0, 3)] * rhs.w;
    float v2 = data[mapIndex(1, 0)] * rhs.x + data[mapIndex(1, 1)] * rhs.y +
               data[mapIndex(1, 2)] * rhs.z + data[mapIndex(1, 3)] * rhs.w;
    float v3 = data[mapIndex(2, 0)] * rhs.x + data[mapIndex(2, 1)] * rhs.y +
               data[mapIndex(2, 2)] * rhs.z + data[mapIndex(2, 3)] * rhs.w;
    float v4 = data[mapIndex(3, 0)] * rhs.x + data[mapIndex(3, 1)] * rhs.y +
               data[mapIndex(3, 2)] * rhs.z + data[mapIndex(3, 3)] * rhs.w;

    return {v1, v2, v3, v4};
  }

  Matrix Transposed() const {
    Matrix retVal;
    for (int row = 0; row < 4; row++)
      for (int col = 0; col < 4; col++)
        retVal.data[mapIndex(col, row)] = data[mapIndex(row, col)];

    return retVal;
  }

  Matrix Inversed() const {
    Matrix retVal{1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
                  0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f};
    Matrix a = *this;

    for (int row = 0; row < 4; row++)
      for (int col = 0; col < 4; col++) {
        if (row == col)
          continue;

        float ratio = a.data[mapIndex(col, row)] / a.data[mapIndex(row, row)];
        a.data[mapIndex(col, 0)] -= a.data[mapIndex(row, 0)] * ratio;
        a.data[mapIndex(col, 1)] -= a.data[mapIndex(row, 1)] * ratio;
        a.data[mapIndex(col, 2)] -= a.data[mapIndex(row, 2)] * ratio;
        a.data[mapIndex(col, 3)] -= a.data[mapIndex(row, 3)] * ratio;

        retVal.data[mapIndex(col, 0)] -= retVal.data[mapIndex(row, 0)] * ratio;
        retVal.data[mapIndex(col, 1)] -= retVal.data[mapIndex(row, 1)] * ratio;
        retVal.data[mapIndex(col, 2)] -= retVal.data[mapIndex(row, 2)] * ratio;
        retVal.data[mapIndex(col, 3)] -= retVal.data[mapIndex(row, 3)] * ratio;
      }

    for (int row = 0; row < 4; row++) {
      float temp = 1 / a.data[mapIndex(row, row)];
      retVal.data[mapIndex(row, 0)] *= temp;
      retVal.data[mapIndex(row, 1)] *= temp;
      retVal.data[mapIndex(row, 2)] *= temp;
      retVal.data[mapIndex(row, 3)] *= temp;
    }

    return retVal;
  }

  Matrix Translated(float x, float y, float z) const noexcept {
    return Matrix{1.f, 0.f, 0.f, x, 0.f, 1.f, 0.f, y,
                  0.f, 0.f, 1.f, z, 0.f, 0.f, 0.f, 1.f} *
           *this;
  }
  Matrix Scaled(float x, float y, float z) const noexcept {
    return Matrix{x,   0.f, 0.f, 0.f, 0.f, y,   0.f, 0.f,
                  0.f, 0.f, z,   0.f, 0.f, 0.f, 0.f, 1.f} *
           *this;
  }
  Matrix Sheared(float Xy = 0, float Xz = 0, float Yx = 0, float Yz = 0,
                 float Zx = 0, float Zy = 0) const noexcept {
    return Matrix{1.f, Xy, Xz,  0.f, Yx,  1.f, Yz,  0.f,
                  Zx,  Zy, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f} *
           *this;
  }
  Matrix RotatedX(float r) const noexcept {
    float c = std::cos(r), s = std::sin(r);

    return Matrix{1.f, 0.f, 0.f, 0.f, 0.f, c,   -s,  0.f,
                  0.f, s,   c,   0.f, 0.f, 0.f, 0.f, 1.f} *
           *this;
  }
  Matrix RotatedY(float r) const noexcept {
    float c = std::cos(r), s = std::sin(r);

    return Matrix{c,  0.f, s, 0.f, 0.f, 1.f, 0.f, 0.f,
                  -s, 0.f, c, 0.f, 0.f, 0.f, 0.f, 1.f} *
           *this;
  }
  Matrix RotatedZ(float r) const noexcept {
    float c = std::cos(r), s = std::sin(r);

    return Matrix{c,   -s,  0.f, 0.f, s,   c,   0.f, 0.f,
                  0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f} *
           *this;
  }
};

} // namespace RayMath