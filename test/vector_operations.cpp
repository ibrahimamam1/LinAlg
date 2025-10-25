#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include "../src/vector.hpp"

class OperationsTest: public testing::Test{
  protected:
    OperationsTest(): u({1,2,3}), v({4,5,6}), w({4.0,5.0,6.0}) {
    }
    VectorND<int>u;
    VectorND<int>v;
    VectorND<double>w;
};

TEST_F(OperationsTest, ScalePositive){
  auto v = u.scale(2);
  EXPECT_EQ(v[0], 2);
  EXPECT_EQ(v[1], 4);
  EXPECT_EQ(v[2], 6);
}

TEST_F(OperationsTest, ScaleNegative){
  auto v = u.scale(-2);
  EXPECT_EQ(v[0], -2);
  EXPECT_EQ(v[1], -4);
  EXPECT_EQ(v[2], -6);
}

TEST_F(OperationsTest, ScaleZero){
  auto v = u.scale(0);
  EXPECT_EQ(v[0], 0);
  EXPECT_EQ(v[1], 0);
  EXPECT_EQ(v[2], 0);
}

TEST_F(OperationsTest, ScaleFractional){
  auto v = u.scale(0.5);
  EXPECT_DOUBLE_EQ(v[0], 0.5);
  EXPECT_DOUBLE_EQ(v[1], 1.0);
  EXPECT_DOUBLE_EQ(v[2], 1.5);
}

TEST_F(OperationsTest, Length){
  double len = u.length();
  double expected = std::sqrt(1*1 + 2*2 + 3*3); // sqrt(14) ≈ 3.7417
  EXPECT_NEAR(len, expected, 0.0001);
}

TEST_F(OperationsTest, LengthAfterScale){
  auto v = u.scale(2);
  double len = v.length();
  double expected = std::sqrt(4 + 16 + 36); // sqrt(56) ≈ 7.4833
  EXPECT_NEAR(len, expected, 0.0001);
}

TEST_F(OperationsTest, LengthOfZeroVector){
  VectorND<int> zero({0, 0, 0});
  EXPECT_DOUBLE_EQ(zero.length(), 0.0);
}

TEST_F(OperationsTest, LengthOfUnitVector){
  VectorND<int> unit({1, 0, 0});
  EXPECT_DOUBLE_EQ(unit.length(), 1.0);
}

TEST_F(OperationsTest, LengthIsAlwaysPositive){
  VectorND<int> neg({-3, -4});
  EXPECT_DOUBLE_EQ(neg.length(), 5.0); // 3-4-5 triangle
}

TEST_F(OperationsTest, ScaleDoesNotChangeDimension){
  int originalDim = u.getDimension();
  auto v = u.scale(5);
  EXPECT_EQ(v.getDimension(), originalDim);
}

TEST_F(OperationsTest, ScaleChaining){
  auto v = u.scale(2);
  auto z = v.scale(3);
  EXPECT_EQ(z[0], 6);
  EXPECT_EQ(z[1], 12);
  EXPECT_EQ(z[2], 18);
}

TEST_F(OperationsTest, LengthWith1DVector){
  VectorND<int> oneDim(std::vector<int>{5});
  EXPECT_DOUBLE_EQ(oneDim.length(), 5.0);
}

TEST_F(OperationsTest, LengthWithHighDimensionalVector){
  VectorND<int> highDim({1, 1, 1, 1, 1}); // 5D
  double expected = std::sqrt(5.0);
  EXPECT_NEAR(highDim.length(), expected, 0.0001);
}

TEST_F(OperationsTest, AddSameTypeVectors){
  auto sum = u.add(v);
  EXPECT_EQ(sum[0], 5); // 1 + 4
  EXPECT_EQ(sum[1], 7); // 2 + 5
  EXPECT_EQ(sum[2], 9); // 3 + 6
}

TEST_F(OperationsTest, AddDifferentTypeVectors){
  auto sum = u.add(w);
  EXPECT_DOUBLE_EQ(sum[0], 5.0); // 1 + 4.0
  EXPECT_DOUBLE_EQ(sum[1], 7.0); // 2 + 5.0
  EXPECT_DOUBLE_EQ(sum[2], 9.0); // 3 + 6.0
}

TEST_F(OperationsTest, SameTypeDotProduct){
  double dp = u.dotProduct(v);
  EXPECT_EQ(dp, 32); // 1*4 + 2*5 + 3*6 = 32
}

TEST_F(OperationsTest, DifferentTypeDotProduct){
  double dp = u.dotProduct(w);
  EXPECT_DOUBLE_EQ(dp, 32.0); // 1*4.0 + 2*5.0 + 3*6.0 = 32.0
}
