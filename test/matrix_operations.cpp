#include "../src/matrix.hpp"
#include <gtest/gtest.h>
#include <cmath>
#include <sstream>

class MatrixOperationsTest : public testing::Test {
protected:
  Matrix<double> A{{{1.0, 2.0}, {3.0, 4.0}}};
  Matrix<double> B{{{5.0, 6.0}, {7.0, 8.0}}};
  Matrix<double> Rect{{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}}}; // 2x3
};

TEST_F(MatrixOperationsTest, AdditionOperator) {
  auto C = A + B;
  EXPECT_EQ(C.getRowDim(), 2);
  EXPECT_EQ(C.getColDim(), 2);
  EXPECT_DOUBLE_EQ(C[0][0], 6.0);
  EXPECT_DOUBLE_EQ(C[0][1], 8.0);
  EXPECT_DOUBLE_EQ(C[1][0], 10.0);
  EXPECT_DOUBLE_EQ(C[1][1], 12.0);

  // Static backward compatibility
  auto C_static = Matrix<double>::add(A, B);
  EXPECT_EQ(C, C_static);

  // Dimension mismatch
  EXPECT_THROW(A + Rect, Matrix<double>::NON_MATCHING_MATRIX_SIZE);
}

TEST_F(MatrixOperationsTest, SubtractionOperator) {
  auto C = B - A;
  EXPECT_EQ(C.getRowDim(), 2);
  EXPECT_EQ(C.getColDim(), 2);
  EXPECT_DOUBLE_EQ(C[0][0], 4.0);
  EXPECT_DOUBLE_EQ(C[0][1], 4.0);
  EXPECT_DOUBLE_EQ(C[1][0], 4.0);
  EXPECT_DOUBLE_EQ(C[1][1], 4.0);

  // Static backward compatibility
  auto C_static = Matrix<double>::subtract(B, A);
  EXPECT_EQ(C, C_static);

  // Dimension mismatch
  EXPECT_THROW(B - Rect, Matrix<double>::NON_MATCHING_MATRIX_SIZE);
}

TEST_F(MatrixOperationsTest, MultiplicationOperator) {
  // A = [1 2; 3 4], B = [5 6; 7 8]
  // A * B = [1*5 + 2*7, 1*6 + 2*8; 3*5 + 4*7, 3*6 + 4*8]
  //       = [19, 22; 43, 50]
  auto C = A * B;
  EXPECT_EQ(C.getRowDim(), 2);
  EXPECT_EQ(C.getColDim(), 2);
  EXPECT_DOUBLE_EQ(C[0][0], 19.0);
  EXPECT_DOUBLE_EQ(C[0][1], 22.0);
  EXPECT_DOUBLE_EQ(C[1][0], 43.0);
  EXPECT_DOUBLE_EQ(C[1][1], 50.0);

  // Static backward compatibility
  auto C_static = Matrix<double>::multiply(A, B);
  EXPECT_EQ(C, C_static);

  // Rectangular multiplication: 2x3 * 3x2 -> 2x2
  Matrix<double> Rect2{{{1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0}}};
  auto Product = Rect * Rect2;
  EXPECT_EQ(Product.getRowDim(), 2);
  EXPECT_EQ(Product.getColDim(), 2);
  // Row 0: 1*1 + 2*3 + 3*5 = 22, 1*2 + 2*4 + 3*6 = 28
  // Row 1: 4*1 + 5*3 + 6*5 = 49, 4*2 + 5*4 + 6*6 = 64
  EXPECT_DOUBLE_EQ(Product[0][0], 22.0);
  EXPECT_DOUBLE_EQ(Product[0][1], 28.0);
  EXPECT_DOUBLE_EQ(Product[1][0], 49.0);
  EXPECT_DOUBLE_EQ(Product[1][1], 64.0);

  // Dimension mismatch
  EXPECT_THROW(Rect * A, Matrix<double>::NON_MATCHING_MATRIX_SIZE);
}

TEST_F(MatrixOperationsTest, ScalarMultiplication) {
  auto C1 = A * 2.0;
  EXPECT_DOUBLE_EQ(C1[0][0], 2.0);
  EXPECT_DOUBLE_EQ(C1[0][1], 4.0);
  EXPECT_DOUBLE_EQ(C1[1][0], 6.0);
  EXPECT_DOUBLE_EQ(C1[1][1], 8.0);

  auto C2 = 3.0 * A;
  EXPECT_DOUBLE_EQ(C2[0][0], 3.0);
  EXPECT_DOUBLE_EQ(C2[0][1], 6.0);
  EXPECT_DOUBLE_EQ(C2[1][0], 9.0);
  EXPECT_DOUBLE_EQ(C2[1][1], 12.0);

  auto C3 = A.scale(2.0);
  EXPECT_EQ(C1, C3);
}

TEST_F(MatrixOperationsTest, VectorMultiplicationAndTransform) {
  VectorND<double> v({2.0, 3.0});
  // A = [1 2; 3 4] * [2; 3] = [1*2 + 2*3; 3*2 + 4*3] = [8; 18]
  auto res1 = A * v;
  EXPECT_EQ(res1.getDimension(), 2);
  EXPECT_DOUBLE_EQ(res1[0], 8.0);
  EXPECT_DOUBLE_EQ(res1[1], 18.0);

  auto res2 = A.transform(v);
  EXPECT_DOUBLE_EQ(res2[0], 8.0);
  EXPECT_DOUBLE_EQ(res2[1], 18.0);

  VectorND<double> v3({1.0, 2.0, 3.0});
  EXPECT_THROW(A * v3, Matrix<double>::NON_MATCHING_MATRIX_SIZE);
  EXPECT_THROW(A.transform(v3), Matrix<double>::NON_MATCHING_MATRIX_SIZE);
}

TEST_F(MatrixOperationsTest, UnaryMinusAndCompoundOperators) {
  auto negA = -A;
  EXPECT_DOUBLE_EQ(negA[0][0], -1.0);
  EXPECT_DOUBLE_EQ(negA[0][1], -2.0);
  EXPECT_DOUBLE_EQ(negA[1][0], -3.0);
  EXPECT_DOUBLE_EQ(negA[1][1], -4.0);

  Matrix<double> M = A;
  M += B;
  EXPECT_DOUBLE_EQ(M[0][0], 6.0);
  EXPECT_DOUBLE_EQ(M[1][1], 12.0);

  M -= B;
  EXPECT_DOUBLE_EQ(M[0][0], 1.0);
  EXPECT_DOUBLE_EQ(M[1][1], 4.0);

  M *= 2.0;
  EXPECT_DOUBLE_EQ(M[0][0], 2.0);
  EXPECT_DOUBLE_EQ(M[1][1], 8.0);
}

TEST_F(MatrixOperationsTest, EqualityOperators) {
  Matrix<double> A_copy = A;
  EXPECT_TRUE(A == A_copy);
  EXPECT_FALSE(A != A_copy);
  EXPECT_FALSE(A == B);
  EXPECT_TRUE(A != B);
}

TEST_F(MatrixOperationsTest, Transpose) {
  // Rect: 2x3 -> Transpose: 3x2
  auto RectT = Rect.transpose();
  EXPECT_EQ(RectT.getRowDim(), 3);
  EXPECT_EQ(RectT.getColDim(), 2);
  EXPECT_DOUBLE_EQ(RectT[0][0], 1.0);
  EXPECT_DOUBLE_EQ(RectT[0][1], 4.0);
  EXPECT_DOUBLE_EQ(RectT[1][0], 2.0);
  EXPECT_DOUBLE_EQ(RectT[1][1], 5.0);
  EXPECT_DOUBLE_EQ(RectT[2][0], 3.0);
  EXPECT_DOUBLE_EQ(RectT[2][1], 6.0);

  // (Rect^T)^T == Rect
  EXPECT_EQ(RectT.transpose(), Rect);
}

TEST_F(MatrixOperationsTest, Determinant) {
  // det(A) = 1*4 - 2*3 = -2
  EXPECT_NEAR(A.det(), -2.0, 1e-9);
  EXPECT_TRUE(A.isInvertible());
  EXPECT_FALSE(A.isSingular());

  // Singular matrix
  Matrix<double> Singular{{{1.0, 2.0}, {2.0, 4.0}}};
  EXPECT_NEAR(Singular.det(), 0.0, 1e-9);
  EXPECT_FALSE(Singular.isInvertible());
  EXPECT_TRUE(Singular.isSingular());

  // 3x3 with zero on diagonal requiring pivoting
  Matrix<double> Pmat{{{0.0, 1.0, 2.0}, {1.0, 0.0, 3.0}, {4.0, -3.0, 8.0}}};
  // det(Pmat) = -0*(...) - 1*(1*8 - 3*4) + 2*(1*-3 - 0*4) = - (8 - 12) + 2*(-3) = 4 - 6 = -2
  EXPECT_NEAR(Pmat.det(), -2.0, 1e-9);

  // Non-square matrix det throws
  EXPECT_THROW(Rect.det(), std::invalid_argument);
}

TEST_F(MatrixOperationsTest, LUFactorization) {
  Matrix<double> M{{{2.0, 1.0}, {6.0, 8.0}}};
  M.factoriseLU();
  const auto& L = M.getL();
  const auto& U = M.getU();
  const auto& P = M.getP();

  // Reconstruct PA from P
  Matrix<double> PA(2, 2);
  for (size_t i = 0; i < 2; ++i) {
    PA[i] = M[P[i]];
  }

  // PA == L * U
  auto LU = L * U;
  EXPECT_EQ(PA, LU);
}

TEST_F(MatrixOperationsTest, Inverse) {
  // A = [1 2; 3 4], det = -2
  // A^-1 = [-2 1; 1.5 -0.5]
  auto A_inv = A.inverse();
  EXPECT_NEAR(A_inv[0][0], -2.0, 1e-9);
  EXPECT_NEAR(A_inv[0][1], 1.0, 1e-9);
  EXPECT_NEAR(A_inv[1][0], 1.5, 1e-9);
  EXPECT_NEAR(A_inv[1][1], -0.5, 1e-9);

  // A * A_inv == I
  auto I = A * A_inv;
  auto I_expected = Matrix<double>::identity(2);
  EXPECT_EQ(I, I_expected);

  // Singular matrix throws
  Matrix<double> Singular{{{1.0, 2.0}, {2.0, 4.0}}};
  EXPECT_THROW(Singular.inverse(), Matrix<double>::NON_INVERTIBLE_MATRIX);

  // Non-square matrix throws
  EXPECT_THROW(Rect.inverse(), Matrix<double>::NON_MATCHING_MATRIX_SIZE);
}

TEST_F(MatrixOperationsTest, Rank) {
  // A is 2x2 full rank
  EXPECT_EQ(A.rank(), 2);

  // Singular 2x2 has rank 1
  Matrix<double> Singular{{{1.0, 2.0}, {2.0, 4.0}}};
  EXPECT_EQ(Singular.rank(), 1);

  // Zero matrix has rank 0
  auto Z = Matrix<double>::zeros(3, 3);
  EXPECT_EQ(Z.rank(), 0);

  // Rectangular 2x3 has rank 2
  EXPECT_EQ(Rect.rank(), 2);
}

TEST_F(MatrixOperationsTest, RowColumnNullSpaces) {
  Matrix<double> M{{{1.0, 2.0, 3.0}, {2.0, 4.0, 6.0}}}; // rank 1
  EXPECT_EQ(M.rank(), 1);

  auto colSpace = M.columnSpace();
  EXPECT_EQ(colSpace.size(), 1); // 1 basis vector
  EXPECT_EQ(colSpace[0].size(), 2);

  auto rowSpace = M.rowSpace();
  EXPECT_EQ(rowSpace.size(), 1); // 1 basis vector
  EXPECT_EQ(rowSpace[0].size(), 3);

  auto nullSpace = M.nullSpace();
  EXPECT_EQ(nullSpace.size(), 2); // Rank-Nullity: 3 - 1 = 2
  for (const auto& basisVec : nullSpace) {
    VectorND<double> v(basisVec);
    auto zeroVec = M * v;
    for (size_t i = 0; i < zeroVec.getDimension(); ++i) {
      EXPECT_NEAR(zeroVec[i], 0.0, 1e-9);
    }
  }
}

TEST_F(MatrixOperationsTest, LinearSystemSolver) {
  // Solve A x = b
  // [1 2; 3 4] [x1; x2] = [5; 11]
  // Solution: x1 = 1, x2 = 2 (1*1 + 2*2 = 5, 3*1 + 4*2 = 11)
  VectorND<double> b({5.0, 11.0});
  auto x = A.solve(b);
  EXPECT_NEAR(x[0], 1.0, 1e-9);
  EXPECT_NEAR(x[1], 2.0, 1e-9);

  // Verify A * x == b
  auto Ax = A * x;
  EXPECT_NEAR(Ax[0], b[0], 1e-9);
  EXPECT_NEAR(Ax[1], b[1], 1e-9);

  // Singular throws
  Matrix<double> Singular{{{1.0, 2.0}, {2.0, 4.0}}};
  EXPECT_THROW(Singular.solve(b), Matrix<double>::NON_INVERTIBLE_MATRIX);
}

TEST_F(MatrixOperationsTest, EigenvaluesAndEigenvectors) {
  // Symmetric matrix: [2 1; 1 2]
  // Eigenvalues: det([2-L 1; 1 2-L]) = (2-L)^2 - 1 = L^2 - 4L + 3 = (L-3)(L-1) = 0
  // L1 = 3, L2 = 1
  Matrix<double> Sym{{{2.0, 1.0}, {1.0, 2.0}}};
  std::vector<double> eigenVals;
  auto eigenVecs = Sym.eigenVectors(eigenVals);

  EXPECT_EQ(eigenVals.size(), 2);
  EXPECT_EQ(eigenVecs.size(), 2);

  // Verify A * v = lambda * v for each pair
  for (size_t i = 0; i < eigenVals.size(); ++i) {
    VectorND<double> v(eigenVecs[i]);
    auto Av = Sym * v;
    auto lv = v.scale(eigenVals[i]);
    for (size_t j = 0; j < v.getDimension(); ++j) {
      EXPECT_NEAR(Av[j], lv[j], 1e-6);
    }
  }
}

TEST_F(MatrixOperationsTest, SVD) {
  Matrix<double> M{{{3.0, 2.0, 2.0}, {2.0, 3.0, -2.0}}};
  Matrix<double> U(2, 2);
  std::vector<double> S;
  Matrix<double> V(3, 3);

  M.svd(U, S, V);

  EXPECT_EQ(S.size(), 2);
  EXPECT_GT(S[0], 0.0);
  EXPECT_GT(S[1], 0.0);

  // Verify reconstructed A = U * S * V^T
  Matrix<double> Sigma(2, 3, 0.0);
  Sigma[0][0] = S[0];
  Sigma[1][1] = S[1];

  auto reconstructed = U * Sigma * V.transpose();
  EXPECT_EQ(reconstructed, M);
}

TEST_F(MatrixOperationsTest, FactoryAndConversions) {
  auto I = Matrix<double>::identity(3);
  EXPECT_DOUBLE_EQ(I[0][0], 1.0);
  EXPECT_DOUBLE_EQ(I[1][1], 1.0);
  EXPECT_DOUBLE_EQ(I[2][2], 1.0);
  EXPECT_DOUBLE_EQ(I[0][1], 0.0);

  auto Z = Matrix<double>::zeros(2, 3);
  EXPECT_DOUBLE_EQ(Z[0][0], 0.0);
  EXPECT_DOUBLE_EQ(Z[1][2], 0.0);

  auto O = Matrix<double>::ones(2, 2);
  EXPECT_DOUBLE_EQ(O[0][0], 1.0);
  EXPECT_DOUBLE_EQ(O[1][1], 1.0);

  auto D = Matrix<double>::diag({2.0, 5.0, 7.0});
  EXPECT_DOUBLE_EQ(D[0][0], 2.0);
  EXPECT_DOUBLE_EQ(D[1][1], 5.0);
  EXPECT_DOUBLE_EQ(D[2][2], 7.0);

  auto vec2d = A.to2DVector();
  EXPECT_DOUBLE_EQ(vec2d[0][0], 1.0);
  EXPECT_DOUBLE_EQ(vec2d[1][1], 4.0);

  auto vec1d = A.to1DVector();
  EXPECT_EQ(vec1d.size(), 4);
  EXPECT_DOUBLE_EQ(vec1d[0], 1.0);
  EXPECT_DOUBLE_EQ(vec1d[1], 2.0);
  EXPECT_DOUBLE_EQ(vec1d[2], 3.0);
  EXPECT_DOUBLE_EQ(vec1d[3], 4.0);

  std::stringstream ss;
  ss << A;
  EXPECT_FALSE(ss.str().empty());
}
