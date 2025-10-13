#include <gtest/gtest.h>
#include "../src/vector.hpp"

TEST(VectorConstructor, NoArgsConstructor){
  VectorND<int>v;
  EXPECT_EQ(v.getDimension(), 1);
  EXPECT_EQ(v.getComponents().size(), 1);
  EXPECT_EQ(v[0], 0);
}
