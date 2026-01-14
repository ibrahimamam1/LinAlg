#include"../src/matrix.hpp"
#include <gtest/gtest.h>

TEST(MatrixConstructorTest, ConstructFromSize){
  Matrix<double> mat(3,4);
  EXPECT_EQ(mat.getRowDim(), 3);
  EXPECT_EQ(mat.getColDim(), 4);
  for(size_t i = 0; i < 3; ++i){
    for(size_t j = 0; j < 4; ++j){
      EXPECT_EQ(mat[i][j], 0.0);
    }
  }
}

TEST(MatrixConstructorTest, ConstructFrom2DVector){
  std::vector<std::vector<double>> values = {
    {1.0, 2.0, 3.0},
    {4.0, 5.0, 6.0}
  };
  Matrix<double> mat(values);
  EXPECT_EQ(mat.getRowDim(), 2);
  EXPECT_EQ(mat.getColDim(), 3);
  EXPECT_EQ(mat[0][0], 1.0);
  EXPECT_EQ(mat[0][1], 2.0);
  EXPECT_EQ(mat[0][2], 3.0);
  EXPECT_EQ(mat[1][0], 4.0);
  EXPECT_EQ(mat[1][1], 5.0);
  EXPECT_EQ(mat[1][2], 6.0);
}

TEST(MatrixConstructorTest, CopyConstructor){
  std::vector<std::vector<double>> values = {
    {1.0, 2.0},
    {3.0, 4.0}
  };
  Matrix<double> mat1(values);
  Matrix<double> mat2(mat1);
  EXPECT_EQ(mat2.getRowDim(), 2);
  EXPECT_EQ(mat2.getColDim(), 2);
  for(size_t i=0; i<mat2.getRowDim(); i++){
    for(size_t j=0; j<mat2.getColDim(); j++){
      EXPECT_EQ(mat2[i][j], mat1[i][j]);
    }
  }
}

TEST(MatrixConstructorTest, AssignmentOperator){
  std::vector<std::vector<double>> values = {
    {1.0, 2.0, 3.0},
    {4.0, 5.0, 6.0}
  };
  Matrix<double> mat1(values);
  Matrix<double> mat2(2,3);
  mat2 = mat1; // Assignment operator
 
  EXPECT_EQ(mat2.getRowDim(), 2);
  EXPECT_EQ(mat2.getColDim(), 2);
  for(size_t i=0; i<mat2.getRowDim(); i++){
    for(size_t j=0; j<mat2.getColDim(); j++){
      EXPECT_EQ(mat2[i][j], mat1[i][j]);
    }
  }}

TEST(MatrixConstructorTest, ElementSetter){
 std::vector<std::vector<double>> values = {
    {1.0, 2.0, 3.0},
    {4.0, 5.0, 6.0}
  };
  Matrix<double> mat(2,3);
  
  mat.setElements(values);
  for(size_t i=0; i<mat.getRowDim(); i++){
    for(size_t j=0; j<mat.getColDim(); j++){
      EXPECT_EQ(mat[i][j], values[i][j]);
    }
  }
}
