#include"../src/matrix.hpp"
#include <gtest/gtest.h>

TEST(MatrixConstructorTest, ConstructFromSize){
  Matrix<double> mat(3,4);
  EXPECT_EQ(mat.getRows(), 3);
  EXPECT_EQ(mat.getColumns(), 4);
}

TEST(MatrixConstructorTest, ConstructFrom2DVector){
  std::vector<std::vector<double>> values = {
    {1.0, 2.0, 3.0},
    {4.0, 5.0, 6.0}
  };
  Matrix<double> mat(values);
  EXPECT_EQ(mat.getRows(), 2);
  EXPECT_EQ(mat.getColumns(), 3);
}

TEST(MatrixConstructorTest, CopyConstructor){
  std::vector<std::vector<double>> values = {
    {1.0, 2.0},
    {3.0, 4.0}
  };
  Matrix<double> mat1(values);
  Matrix<double> mat2 = mat1; // Copy constructor
  EXPECT_EQ(mat2.getRows(), 2);
  EXPECT_EQ(mat2.getColumns(), 2);
}

TEST(MatrixConstructorTest, AssignmentOperator){
  std::vector<std::vector<double>> values = {
    {1.0, 2.0, 3.0},
    {4.0, 5.0, 6.0}
  };
  Matrix<double> mat1(values);
  Matrix<double> mat2(2,3);
  mat2 = mat1; // Assignment operator
  EXPECT_EQ(mat2.getRows(), 2);
  EXPECT_EQ(mat2.getColumns(), 3);
}
