#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>
#include "../src/vector.hpp"

TEST(VectorConstructor, NoArgsConstructor){
  VectorND<int>v;
  EXPECT_EQ(v.getDimension(), 1);
  EXPECT_EQ(v.getComponents().size(), 1);
  EXPECT_EQ(v[0], 0);
}

TEST(VectorConstructor, DimensionArgConstructor1){
  VectorND<int>v(3);
  EXPECT_EQ(v.getDimension(), 3);
  EXPECT_EQ(v.getComponents().size(), 3);
  EXPECT_EQ(v[0], 0);
  EXPECT_EQ(v[1], 0);
  EXPECT_EQ(v[2], 0);

  VectorND<float>u(3);
  EXPECT_EQ(u.getDimension(), 3);
  EXPECT_EQ(u.getComponents().size(), 3);
  EXPECT_EQ(u[0], 0.0);
  EXPECT_EQ(u[1], 0.0);
  EXPECT_EQ(u[2], 0.0);
}

TEST(VectorConstructor, StdVectorArgsConstructor2){
  std::vector<int>x = {1,2,3};
  VectorND<int>v(x);
  EXPECT_EQ(v.getDimension(), 3);
  EXPECT_EQ(v.getComponents().size(), 3);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
}

TEST(VectorConstructor, CopyConstructor){
  std::vector<int>x = {1,2,3};
  
  VectorND<int>v(x);
  VectorND<int>u(v);

  EXPECT_EQ(u.getDimension(), v.getDimension());
  EXPECT_EQ(u.getComponents().size(), v.getComponents().size());
  EXPECT_EQ(u[0], v[0]);
  EXPECT_EQ(u[1], v[1]);
  EXPECT_EQ(u[2], v[2]);
}

TEST(VectorConstructor, SetComponents){
  VectorND<int>v(3);
  std::vector<int> x = {1,2,3,4};
  std::vector<int> y = {1,2};
  std::vector<int> z = {1,2,3};
  
  v.setComponents(x);
  EXPECT_EQ(v[0], x[0]);
  EXPECT_EQ(v[1], x[1]);
  EXPECT_EQ(v[2], x[2]);
  EXPECT_EQ(v.getDimension(), 3);
  
  v.setComponents(y);
  EXPECT_EQ(v[0], y[0]);
  EXPECT_EQ(v[1], y[1]);
  EXPECT_EQ(v[2], 0);

  v.setComponents(z);
  EXPECT_EQ(v[0], z[0]);
  EXPECT_EQ(v[1], z[1]);
  EXPECT_EQ(v[2], z[2]);
}
