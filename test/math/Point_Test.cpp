
#include "Point.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class PointTests : public testing::Test {};
using namespace RayMath;
TEST_F(PointTests, DefaultCreationTest) {
  Point point;

  EXPECT_EQ(point.x, 0);
  EXPECT_EQ(point.y, 0);
  EXPECT_EQ(point.z, 0);
  EXPECT_EQ(point.w, 1);
}

TEST_F(PointTests, NonDefaultCreationTest) {
  Point point{1, 2, 3, 4};

  EXPECT_EQ(point.x, 1);
  EXPECT_EQ(point.y, 2);
  EXPECT_EQ(point.z, 3);
  EXPECT_EQ(point.w, 4);
}

TEST_F(PointTests, PointToVectorAdditionTest_default) {
  Point point{1, 2, 3};
  Vector vector{4, 3, 2};

  Point result = point + vector;

  EXPECT_EQ(result, Point(5, 5, 5));
}

TEST_F(PointTests, PointToVectorAdditionTest_4D) {
  Point point{1, 2, 3, 4};
  Vector vector{4, 3, 2, 1};

  Point result = point + vector;

  EXPECT_EQ(result, Point(5, 5, 5, 5));
}

TEST_F(PointTests, PointToVectorSubtractionTest_default) {
  Point point{3, 2, 1};
  Vector vector{5, 6, 7};

  Point result = point - vector;

  EXPECT_EQ(result, Point(-2, -4, -6, 1));
}

TEST_F(PointTests, PointToVectorSubtractionTest_4D) {
  Point point{3, 2, 1, 4};
  Vector vector{5, 6, 7, 1};

  Point result = point - vector;

  EXPECT_EQ(result, Point(-2, -4, -6, 3));
}

TEST_F(PointTests, PointToPointSubtractionTest) {
  Point point_1{3, 2, 1};
  Point point_2{5, 6, 7};

  Vector result = point_1 - point_2;

  EXPECT_EQ(result, Vector(-2, -4, -6, 0));
}

TEST_F(PointTests, PointNegationTest) {
  Point point{3, 2, 1};

  Point result = -point;

  EXPECT_EQ(result, Point(-3, -2, -1, -1));
}