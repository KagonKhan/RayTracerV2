
#include "Vector.hpp"
#include "gtest/gtest.h"

class VectorTests : public testing::Test {};

TEST_F(VectorTests, DefaultCreationTest) {
  RayMath::Vector vector;

  EXPECT_EQ(vector.x, 0);
  EXPECT_EQ(vector.y, 0);
  EXPECT_EQ(vector.z, 0);
  EXPECT_EQ(vector.w, 0);
}

TEST_F(VectorTests, NonDefaultCreationTest) {
  RayMath::Vector vector{1, 2, 3, 4};

  EXPECT_EQ(vector.x, 1);
  EXPECT_EQ(vector.y, 2);
  EXPECT_EQ(vector.z, 3);
  EXPECT_EQ(vector.w, 4);
}