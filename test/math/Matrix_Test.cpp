#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "Matrix.hpp"

class MatrixTests : public testing::Test {};
using namespace RayMath;
using namespace testing;

TEST_F(MatrixTests, DefaultCreationTest) {
  Matrix matrix;

  ASSERT_EQ(matrix.data.size(), 16);
  ASSERT_THAT(matrix.data, Each(Eq(0.f)));
}

TEST_F(MatrixTests, NonDefaultCreationTest) {
  Matrix matrix{1.f, 2.f,  3.f,  4.f,  5.f,  6.f,  7.f,  8.f,
                9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f};

  ASSERT_THAT(matrix.data,
              ElementsAre(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f,
                          11.f, 12.f, 13.f, 14.f, 15.f, 16.f));
}

TEST_F(MatrixTests, MatrixComparisonTest) {
  Matrix m1(1.f, 2.f, 3.f, 4.f, 5.5f, 6.5f, 7.5f, 8.5f, 9.f, 10.f, 11.f, 12.f,
            13.5f, 14.5f, 15.5f, 16.5f);

  Matrix m2(1.f, 2.f, 3.f, 4.f, 5.5f, 6.5f, 7.5f, 8.5f, 9.f, 10.f, 11.f, 12.f,
            13.5f, 14.5f, 15.5f, 16.5f);

  Matrix m3(1.f, 2.f, 3.f, 4.f, 5.5f, 6.5f, 7.5f, 8.5f, 9.f, 10.f, 12.f, 12.f,
            13.5f, 14.5f, 15.5f, 16.5f);

  ASSERT_EQ(m1, m2);
  ASSERT_FALSE(m1 == m3);
}

TEST_F(MatrixTests, MatrixToMatrixMultiplicationTest) {
  Matrix m1(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 8.f, 7.f, 6.f, 5.f,
            4.f, 3.f, 2.f);

  Matrix m2(-2.f, 1.f, 2.f, 3.f, 3.f, 2.f, 1.f, -1.f, 4.f, 3.f, 6.f, 5.f, 1.f,
            2.f, 7.f, 8.f);

  Matrix res(20.f, 22.f, 50.f, 48.f, 44.f, 54.f, 114.f, 108.f, 40.f, 58.f,
             110.f, 102.f, 16.f, 26.f, 46.f, 42.f);

  ASSERT_EQ(res, m1 * m2);
}

TEST_F(MatrixTests, MatrixToPointMultiplicationTest) {
  Matrix m(1.f, 2.f, 3.f, 4.f, 2.f, 4.f, 4.f, 2.f, 8.f, 6.f, 4.f, 1.f, 0.f, 0.f,
           0.f, 1.f);

  Point p(1.f, 2.f, 3.f);

  ASSERT_EQ(m * p, Point(18.f, 24.f, 33.f, 1.f));
}

TEST_F(MatrixTests, MatrixTranspositionTest) {
  Matrix m1(0.f, 9.f, 3.f, 0.f, 9.f, 8.f, 0.f, 8.f, 1.f, 8.f, 5.f, 3.f, 0.f,
            0.f, 5.f, 8.f);

  Matrix m2(0.f, 9.f, 1.f, 0.f, 9.f, 8.f, 8.f, 0.f, 3.f, 0.f, 5.f, 5.f, 0.f,
            8.f, 3.f, 8.f);

  ASSERT_EQ(m1.Transposed(), m2);
}

TEST_F(MatrixTests, MatrixInverseTest) {
  Matrix m(-5.f, 2.f, 6.f, -8.f, 1.f, -5.f, 1.f, 8.f, 7.f, 7.f, -6.f, -7.f, 1.f,
           -3.f, 7.f, 4.f);

  Matrix expected(0.21805f, 0.45113f, 0.24060f, -0.04511f, -0.80827f, -1.45677f,
                  -0.44361f, 0.52068f, -0.07895f, -0.22368f, -0.05263f,
                  0.19737f, -0.52256f, -0.81391f, -0.30075f, 0.30639f);

  ASSERT_EQ(m.Inversed(), expected);
}