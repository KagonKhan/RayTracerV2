
#include "Vector.hpp"
#include "gtest/gtest.h"

class VectorTests : public testing::Test {};
using namespace RayMath;

TEST_F(VectorTests, DefaultCreationTest) {
  Vector vector;

  EXPECT_EQ(vector.x, 0);
  EXPECT_EQ(vector.y, 0);
  EXPECT_EQ(vector.z, 0);
  EXPECT_EQ(vector.w, 0);
}

TEST_F(VectorTests, NonDefaultCreationTest) {
  Vector vector{1, 2, 3, 4};

  EXPECT_EQ(vector.x, 1);
  EXPECT_EQ(vector.y, 2);
  EXPECT_EQ(vector.z, 3);
  EXPECT_EQ(vector.w, 4);
}

TEST_F(VectorTests, VectorToVectorAdditionTest) {
  Vector vector_v1{1, 2, 3};
  Vector vector_v2{4, 3, 2};

  Vector result = vector_v1 + vector_v2;

  EXPECT_EQ(result, Vector(5, 5, 5));
}

TEST_F(VectorTests, VectorToVectorSubtractionTest) {
  Vector vector_v1{1, 2, 3};
  Vector vector_v2{4, 3, 2};

  Vector result = vector_v1 - vector_v2;

  EXPECT_EQ(result, Vector(-3, -1, 1));
}

TEST_F(VectorTests, VectorNegationTest) {
  Vector vector{-1, -2, -3};

  EXPECT_EQ(-vector, Vector(1, 2, 3));
}

TEST_F(VectorTests, VectorScalarMultiplicationTest) {
  Vector vector{-1, -2, -3};

  EXPECT_EQ(vector * 2, Vector(-2, -4, -6));
}

TEST_F(VectorTests, VectorScalarDivisionTest) {
  Vector vector{-1, -2, -3};

  EXPECT_EQ(vector / 2.f, Vector(-0.5f, -1.f, -1.5f));
}

TEST_F(VectorTests, VectorMagnitudeTest) {
  Vector vector{-1, -2, -3};

  EXPECT_EQ(vector.magnitude(), sqrtf(14.f));
}

TEST_F(VectorTests, VectorNormalizationTest) {
  Vector vector{4, 0, 0};

  EXPECT_EQ(vector.normalized(), Vector(1, 0, 0));
}

TEST_F(VectorTests, VectorDotProductTest) {
  Vector vector_v1{1, 2, 3};
  Vector vector_v2{2, 3, 4};

  EXPECT_EQ(vector_v1.dot(vector_v2), 20.f);
}

TEST_F(VectorTests, VectorCrossProductTest) {
  Vector vector_v1{1, 2, 3};
  Vector vector_v2{2, 3, 4};

  EXPECT_EQ(vector_v1.cross(vector_v2), Vector(-1, 2, -1, 0));
  EXPECT_EQ(vector_v2.cross(vector_v1), Vector(1, -2, 1, 0));
}
