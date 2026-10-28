#ifndef MATRIX_H
#define MATRIX_H

#include "vector.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

template <typename T> class Matrix {
private:
  size_t _m; // Number of Rows
  size_t _n; // Number of Columns
  std::vector<std::vector<T>> elements;

  mutable std::unique_ptr<Matrix<T>> L = nullptr; // Lower triangular matrix
  mutable std::unique_ptr<Matrix<T>> U = nullptr; // Upper triangular matrix
  mutable std::vector<size_t> P;                  // Permutation indices
  mutable size_t numRowSwaps = 0;

  mutable bool luFactorized = false;
  mutable double determinant = 0.0;
  mutable bool determinantComputed = false;
  mutable size_t rankVal = 0;
  mutable bool rankComputed = false;


  void computeLU() const {
    if (luFactorized && L && U) {
      return;
    }
    if (_m != _n) {
      throw std::invalid_argument("LU factorization requires a square matrix");
    }

    size_t n = _m;
    L = std::make_unique<Matrix<T>>(n, n);
    U = std::make_unique<Matrix<T>>(*this);

    // Initialize L as identity matrix
    for (size_t i = 0; i < n; ++i) {
      for (size_t j = 0; j < n; ++j) {
        (*L)[i][j] = (i == j) ? static_cast<T>(1) : static_cast<T>(0);
      }
    }

    P.resize(n);
    for (size_t i = 0; i < n; ++i) {
      P[i] = i;
    }
    numRowSwaps = 0;

    for (size_t k = 0; k < n; ++k) {
      // Partial pivoting: find maximum pivot in column k at or below row k
      size_t pivotRow = k;
      double maxVal = std::abs(static_cast<double>((*U)[k][k]));
      for (size_t i = k + 1; i < n; ++i) {
        double val = std::abs(static_cast<double>((*U)[i][k]));
        if (val > maxVal) {
          maxVal = val;
          pivotRow = i;
        }
      }

      if (pivotRow != k) {
        // Swap rows in U
        std::swap((*U)[k], (*U)[pivotRow]);
        // Swap already computed multipliers in L (columns 0 to k-1)
        for (size_t c = 0; c < k; ++c) {
          std::swap((*L)[k][c], (*L)[pivotRow][c]);
        }
        // Swap permutation indices
        std::swap(P[k], P[pivotRow]);
        numRowSwaps++;
      }

      if (std::abs(static_cast<double>((*U)[k][k])) < 1e-12) {
        continue;
      }

      for (size_t i = k + 1; i < n; ++i) {
        T factor = (*U)[i][k] / (*U)[k][k];
        (*L)[i][k] = factor;
        (*U)[i][k] = static_cast<T>(0);
        for (size_t j = k + 1; j < n; ++j) {
          (*U)[i][j] -= factor * (*U)[k][j];
        }
      }
    }

    luFactorized = true;
  }

  std::vector<std::vector<double>> computeRREF(std::vector<size_t>& pivotCols) const {
    pivotCols.clear();
    std::vector<std::vector<double>> mat(_m, std::vector<double>(_n, 0.0));
    for (size_t i = 0; i < _m; ++i) {
      for (size_t j = 0; j < _n; ++j) {
        mat[i][j] = static_cast<double>(elements[i][j]);
      }
    }

    size_t lead = 0;
    for (size_t r = 0; r < _m; ++r) {
      if (lead >= _n) break;
      size_t i = r;
      while (std::abs(mat[i][lead]) < 1e-9) {
        i++;
        if (i == _m) {
          i = r;
          lead++;
          if (lead == _n) break;
        }
      }
      if (lead == _n) break;

      std::swap(mat[i], mat[r]);
      double val = mat[r][lead];
      for (size_t j = 0; j < _n; ++j) {
        mat[r][j] /= val;
      }
      for (size_t k = 0; k < _m; ++k) {
        if (k != r) {
          double factor = mat[k][lead];
          for (size_t j = 0; j < _n; ++j) {
            mat[k][j] -= factor * mat[r][j];
          }
        }
      }
      pivotCols.push_back(lead);
      lead++;
    }

    for (size_t i = 0; i < _m; ++i) {
      for (size_t j = 0; j < _n; ++j) {
        if (std::abs(mat[i][j]) < 1e-9) {
          mat[i][j] = 0.0;
        }
      }
    }
    return mat;
  }

public:
  class NON_INVERTIBLE_MATRIX : public std::runtime_error {
  public:
    NON_INVERTIBLE_MATRIX(const std::string& msg = "Matrix is non-invertible")
        : std::runtime_error(msg) {}
  };

  class NON_MATCHING_MATRIX_SIZE : public std::invalid_argument {
  public:
    NON_MATCHING_MATRIX_SIZE(const std::string& msg = "Matrix size mismatch")
        : std::invalid_argument(msg) {}
  };

  Matrix() = delete;

  Matrix(size_t m, size_t n, T initVal = static_cast<T>(0))
      : _m(m), _n(n), elements(m, std::vector<T>(n, initVal)) {}

  Matrix(const std::vector<std::vector<T>> &values) {
    if (values.empty()) {
      _m = 0;
      _n = 0;
    } else {
      _m = values.size();
      _n = values[0].size();
      elements = values;
    }
  }

  // Copy Constructor
  Matrix(const Matrix<T> &other)
      : _m(other._m), _n(other._n), elements(other.elements),
        P(other.P), numRowSwaps(other.numRowSwaps),
        luFactorized(other.luFactorized), determinant(other.determinant),
        determinantComputed(other.determinantComputed),
        rankVal(other.rankVal), rankComputed(other.rankComputed) {
    if (other.L) L = std::make_unique<Matrix<T>>(*other.L);
    if (other.U) U = std::make_unique<Matrix<T>>(*other.U);
  }

  // Copy Assignment Operator
  Matrix<T> &operator=(const Matrix<T> &other) {
    if (this != &other) {
      _m = other._m;
      _n = other._n;
      elements = other.elements;
      P = other.P;
      numRowSwaps = other.numRowSwaps;
      luFactorized = other.luFactorized;
      determinant = other.determinant;
      determinantComputed = other.determinantComputed;
      rankVal = other.rankVal;
      rankComputed = other.rankComputed;
      L = other.L ? std::make_unique<Matrix<T>>(*other.L) : nullptr;
      U = other.U ? std::make_unique<Matrix<T>>(*other.U) : nullptr;
    }
    return *this;
  }

  // Move Constructor
  Matrix(Matrix<T> &&) noexcept = default;

  // Move Assignment Operator
  Matrix<T> &operator=(Matrix<T> &&) noexcept = default;

  // Destructor
  ~Matrix() = default;

  Matrix<T> &operator=(const std::vector<std::vector<T>> &values) {
    if (values.empty()) {
      this->_m = 0;
      this->_n = 0;
      this->elements.clear();
    } else {
      this->_m = values.size();
      this->_n = values[0].size();
      this->elements = values;
    }
    invalidateCache();
    return *this;
  }

  void setElements(const std::vector<std::vector<T>>& values) {
    if (values.empty()) {
      this->_m = 0;
      this->_n = 0;
      this->elements.clear();
    } else {
      this->_m = values.size();
      this->_n = values[0].size();
      this->elements = values;
    }
    invalidateCache();
  }

  std::vector<T> &operator[](size_t i) {
    if (i >= _m) {
      throw std::out_of_range("Index out of range\n");
    }
    return elements[i];
  }

  const std::vector<T> &operator[](size_t i) const {
    if (i >= _m) {
      throw std::out_of_range("Index out of range\n");
    }
    return elements[i];
  }

  T &at(size_t i, size_t j) {
    if (i >= _m || j >= _n) {
      throw std::out_of_range("Matrix index out of range");
    }
    return elements[i][j];
  }

  const T &at(size_t i, size_t j) const {
    if (i >= _m || j >= _n) {
      throw std::out_of_range("Matrix index out of range");
    }
    return elements[i][j];
  }

  size_t getRowDim() const { return _m; }
  size_t getColDim() const { return _n; }
  size_t getRows() const { return _m; }
  size_t getCols() const { return _n; }

  void invalidateCache() const {
    luFactorized = false;
    L.reset();
    U.reset();
    P.clear();
    numRowSwaps = 0;
    determinantComputed = false;
    rankComputed = false;
  }


  // Factory methods
  static Matrix<T> identity(size_t size) {
    Matrix<T> res(size, size, static_cast<T>(0));
    for (size_t i = 0; i < size; ++i) {
      res[i][i] = static_cast<T>(1);
    }
    return res;
  }

  static Matrix<T> zeros(size_t m, size_t n) {
    return Matrix<T>(m, n, static_cast<T>(0));
  }

  static Matrix<T> ones(size_t m, size_t n) {
    return Matrix<T>(m, n, static_cast<T>(1));
  }

  static Matrix<T> diag(const std::vector<T>& values) {
    size_t size = values.size();
    Matrix<T> res(size, size, static_cast<T>(0));
    for (size_t i = 0; i < size; ++i) {
      res[i][i] = values[i];
    }
    return res;
  }

  // Overloaded Operators for Add, Subtract, Multiply

  // Matrix + Matrix
  template <typename U>
  auto operator+(const Matrix<U>& other) const -> Matrix<decltype(std::declval<T>() + std::declval<U>())> {
    if (_m != other.getRowDim() || _n != other.getColDim()) {
      throw NON_MATCHING_MATRIX_SIZE("Matrix dimension mismatch for addition");
    }
    using ResultType = decltype(std::declval<T>() + std::declval<U>());
    Matrix<ResultType> res(_m, _n);
    for (size_t i = 0; i < _m; ++i) {
      for (size_t j = 0; j < _n; ++j) {
        res[i][j] = elements[i][j] + other[i][j];
      }
    }
    return res;
  }

  // Matrix - Matrix
  template <typename U>
  auto operator-(const Matrix<U>& other) const -> Matrix<decltype(std::declval<T>() - std::declval<U>())> {
    if (_m != other.getRowDim() || _n != other.getColDim()) {
      throw NON_MATCHING_MATRIX_SIZE("Matrix dimension mismatch for subtraction");
    }
    using ResultType = decltype(std::declval<T>() - std::declval<U>());
    Matrix<ResultType> res(_m, _n);
    for (size_t i = 0; i < _m; ++i) {
      for (size_t j = 0; j < _n; ++j) {
        res[i][j] = elements[i][j] - other[i][j];
      }
    }
    return res;
  }

  // Matrix * Matrix
  template <typename U>
  auto operator*(const Matrix<U>& other) const -> Matrix<decltype(std::declval<T>() * std::declval<U>())> {
    if (_n != other.getRowDim()) {
      throw NON_MATCHING_MATRIX_SIZE("Matrix dimension mismatch for multiplication");
    }
    using ResultType = decltype(std::declval<T>() * std::declval<U>());
    size_t p = other.getColDim();
    Matrix<ResultType> res(_m, p, static_cast<ResultType>(0));
    for (size_t i = 0; i < _m; ++i) {
      for (size_t k = 0; k < _n; ++k) {
        for (size_t j = 0; j < p; ++j) {
          res[i][j] += elements[i][k] * other[k][j];
        }
      }
    }
    return res;
  }

  // Matrix * VectorND
  template <typename V>
  auto operator*(const VectorND<V>& vec) const -> VectorND<decltype(std::declval<T>() * std::declval<V>())> {
    if (_n != vec.getDimension()) {
      throw NON_MATCHING_MATRIX_SIZE("Dimension mismatch for matrix-vector multiplication");
    }
    using ResultType = decltype(std::declval<T>() * std::declval<V>());
    std::vector<ResultType> res(_m, static_cast<ResultType>(0));
    for (size_t i = 0; i < _m; ++i) {
      for (size_t j = 0; j < _n; ++j) {
        res[i] += elements[i][j] * vec[j];
      }
    }
    return VectorND<ResultType>(res);
  }

  // Matrix * scalar
  template <typename S, typename = std::enable_if_t<std::is_arithmetic_v<S>>>
  auto operator*(const S scalar) const -> Matrix<decltype(std::declval<T>() * std::declval<S>())> {
    using ResultType = decltype(std::declval<T>() * std::declval<S>());
    Matrix<ResultType> res(_m, _n);
    for (size_t i = 0; i < _m; ++i) {
      for (size_t j = 0; j < _n; ++j) {
        res[i][j] = elements[i][j] * scalar;
      }
    }
    return res;
  }

  // scalar * Matrix
  template <typename S, typename = std::enable_if_t<std::is_arithmetic_v<S>>>
  friend auto operator*(const S scalar, const Matrix<T>& mat) -> Matrix<decltype(std::declval<S>() * std::declval<T>())> {
    return mat * scalar;
  }

  // Unary minus -Matrix
  Matrix<T> operator-() const {
    Matrix<T> res(_m, _n);
    for (size_t i = 0; i < _m; ++i) {
      for (size_t j = 0; j < _n; ++j) {
        res[i][j] = -elements[i][j];
      }
    }
    return res;
  }

  // Compound assignment operators
  template <typename U>
  Matrix<T>& operator+=(const Matrix<U>& other) {
    if (_m != other.getRowDim() || _n != other.getColDim()) {
      throw NON_MATCHING_MATRIX_SIZE("Matrix dimension mismatch for +=");
    }
    invalidateCache();
    for (size_t i = 0; i < _m; ++i) {
      for (size_t j = 0; j < _n; ++j) {
        elements[i][j] += other[i][j];
      }
    }
    return *this;
  }

  template <typename U>
  Matrix<T>& operator-=(const Matrix<U>& other) {
    if (_m != other.getRowDim() || _n != other.getColDim()) {
      throw NON_MATCHING_MATRIX_SIZE("Matrix dimension mismatch for -=");
    }
    invalidateCache();
    for (size_t i = 0; i < _m; ++i) {
      for (size_t j = 0; j < _n; ++j) {
        elements[i][j] -= other[i][j];
      }
    }
    return *this;
  }

  template <typename U>
  Matrix<T>& operator*=(const Matrix<U>& other) {
    *this = *this * other;
    return *this;
  }

  template <typename S, typename = std::enable_if_t<std::is_arithmetic_v<S>>>
  Matrix<T>& operator*=(const S scalar) {
    invalidateCache();
    for (size_t i = 0; i < _m; ++i) {
      for (size_t j = 0; j < _n; ++j) {
        elements[i][j] *= scalar;
      }
    }
    return *this;
  }

  // Equality Operators
  template <typename U>
  bool operator==(const Matrix<U>& other) const {
    if (_m != other.getRowDim() || _n != other.getColDim()) {
      return false;
    }
    for (size_t i = 0; i < _m; ++i) {
      for (size_t j = 0; j < _n; ++j) {
        if constexpr (std::is_floating_point_v<T> || std::is_floating_point_v<U>) {
          if (std::abs(static_cast<double>(elements[i][j]) - static_cast<double>(other[i][j])) > 1e-9) {
            return false;
          }
        } else {
          if (elements[i][j] != other[i][j]) {
            return false;
          }
        }
      }
    }
    return true;
  }

  template <typename U>
  bool operator!=(const Matrix<U>& other) const {
    return !(*this == other);
  }

  // Backwards compatibility static methods
  static Matrix<T> add(const Matrix<T>& a, const Matrix<T>& b) {
    return a + b;
  }

  static Matrix<T> subtract(const Matrix<T>& a, const Matrix<T>& b) {
    return a - b;
  }

  static Matrix<T> multiply(const Matrix<T>& a, const Matrix<T>& b) {
    return a * b;
  }

  // Conversions
  std::vector<std::vector<T>> to2DVector() const {
    return elements;
  }

  std::vector<T> to1DVector() const {
    std::vector<T> flat;
    flat.reserve(_m * _n);
    for (size_t i = 0; i < _m; ++i) {
      for (size_t j = 0; j < _n; ++j) {
        flat.push_back(elements[i][j]);
      }
    }
    return flat;
  }

  // Transpose
  Matrix<T> transpose() const {
    Matrix<T> res(_n, _m);
    for (size_t i = 0; i < _m; ++i) {
      for (size_t j = 0; j < _n; ++j) {
        res[j][i] = elements[i][j];
      }
    }
    return res;
  }

  // Scale
  template <typename S>
  auto scale(const S scalar) const {
    return *this * scalar;
  }

  // Transform vector
  template <typename V>
  auto transform(const VectorND<V>& x) const {
    return *this * x;
  }

  // LU Factorization
  Matrix<T> &factoriseLU() {
    computeLU();
    return *this;
  }

  Matrix<T> &factoriseLU(Matrix<T> &outU) {
    computeLU();
    outU = *this->U;
    return *this->L;
  }

  const Matrix<T> &getL() const {
    computeLU();
    return *L;
  }

  const Matrix<T> &getU() const {
    computeLU();
    return *U;
  }

  const std::vector<size_t> &getP() const {
    computeLU();
    return P;
  }

  // Determinant & Invertibility
  double det() const {
    if (determinantComputed) {
      return determinant;
    }
    if (_m != _n) {
      throw std::invalid_argument("Determinant only exists for square matrices");
    }

    computeLU();

    double detValue = (numRowSwaps % 2 == 1) ? -1.0 : 1.0;
    for (size_t i = 0; i < _n; ++i) {
      detValue *= static_cast<double>((*U)[i][i]);
    }

    if (std::abs(detValue) < 1e-12) {
      detValue = 0.0;
    }

    determinant = detValue;
    determinantComputed = true;
    return determinant;
  }

  bool isInvertible() const {
    if (determinantComputed) {
      return determinant != 0.0;
    }
    return std::abs(det()) > 1e-12;
  }

  bool isSingular() const {
    return !isInvertible();
  }

  // Inverse
  template <typename ResT = std::conditional_t<std::is_floating_point_v<T>, T, double>>
  Matrix<ResT> inverse() const {
    if (_m != _n) {
      throw NON_MATCHING_MATRIX_SIZE("Inverse only exists for square matrices");
    }
    if (!isInvertible()) {
      throw NON_INVERTIBLE_MATRIX("Matrix is singular and cannot be inverted");
    }

    size_t n = _m;
    std::vector<std::vector<double>> aug(n, std::vector<double>(2 * n, 0.0));
    for (size_t i = 0; i < n; ++i) {
      for (size_t j = 0; j < n; ++j) {
        aug[i][j] = static_cast<double>(elements[i][j]);
      }
      aug[i][n + i] = 1.0;
    }

    for (size_t i = 0; i < n; ++i) {
      size_t maxRow = i;
      double maxVal = std::abs(aug[i][i]);
      for (size_t k = i + 1; k < n; ++k) {
        if (std::abs(aug[k][i]) > maxVal) {
          maxVal = std::abs(aug[k][i]);
          maxRow = k;
        }
      }

      if (maxVal < 1e-12) {
        throw NON_INVERTIBLE_MATRIX("Matrix is singular and cannot be inverted");
      }

      std::swap(aug[i], aug[maxRow]);

      double pivot = aug[i][i];
      for (size_t j = 0; j < 2 * n; ++j) {
        aug[i][j] /= pivot;
      }

      for (size_t k = 0; k < n; ++k) {
        if (k != i) {
          double factor = aug[k][i];
          for (size_t j = 0; j < 2 * n; ++j) {
            aug[k][j] -= factor * aug[i][j];
          }
        }
      }
    }

    std::vector<std::vector<ResT>> invElem(n, std::vector<ResT>(n));
    for (size_t i = 0; i < n; ++i) {
      for (size_t j = 0; j < n; ++j) {
        invElem[i][j] = static_cast<ResT>(aug[i][n + j]);
      }
    }
    return Matrix<ResT>(invElem);
  }

  // Rank
  size_t rank() const {
    if (rankComputed) {
      return rankVal;
    }
    std::vector<size_t> pivotCols;
    computeRREF(pivotCols);
    rankVal = pivotCols.size();
    rankComputed = true;
    return rankVal;
  }

  // Column Space Basis
  std::vector<std::vector<T>> columnSpace() const {
    std::vector<size_t> pivotCols;
    computeRREF(pivotCols);
    std::vector<std::vector<T>> basis;
    for (size_t colIdx : pivotCols) {
      std::vector<T> col(_m);
      for (size_t i = 0; i < _m; ++i) {
        col[i] = elements[i][colIdx];
      }
      basis.push_back(col);
    }
    return basis;
  }

  // Row Space Basis
  std::vector<std::vector<T>> rowSpace() const {
    std::vector<size_t> pivotCols;
    auto rrefMat = computeRREF(pivotCols);
    std::vector<std::vector<T>> basis;
    for (size_t r = 0; r < pivotCols.size(); ++r) {
      std::vector<T> row(_n);
      for (size_t j = 0; j < _n; ++j) {
        row[j] = static_cast<T>(rrefMat[r][j]);
      }
      basis.push_back(row);
    }
    return basis;
  }

  // Null Space Basis
  std::vector<std::vector<T>> nullSpace() const {
    std::vector<size_t> pivotCols;
    auto rrefMat = computeRREF(pivotCols);
    std::vector<bool> isPivot(_n, false);
    for (size_t p : pivotCols) {
      isPivot[p] = true;
    }

    std::vector<std::vector<T>> basis;
    for (size_t f = 0; f < _n; ++f) {
      if (!isPivot[f]) {
        std::vector<T> v(_n, static_cast<T>(0));
        v[f] = static_cast<T>(1);
        for (size_t r = 0; r < pivotCols.size(); ++r) {
          size_t pCol = pivotCols[r];
          v[pCol] = static_cast<T>(-rrefMat[r][f]);
        }
        basis.push_back(v);
      }
    }
    return basis;
  }

  // Linear System Solver Ax = b
  VectorND<double> solve(const VectorND<T>& b) const {
    if (_m != _n) {
      throw NON_MATCHING_MATRIX_SIZE("Matrix must be square to solve Ax = b with unique solution");
    }
    if (_m != b.getDimension()) {
      throw NON_MATCHING_MATRIX_SIZE("Matrix row count must match vector dimension");
    }

    size_t n = _m;
    std::vector<std::vector<double>> aug(n, std::vector<double>(n + 1));
    for (size_t i = 0; i < n; ++i) {
      for (size_t j = 0; j < n; ++j) {
        aug[i][j] = static_cast<double>(elements[i][j]);
      }
      aug[i][n] = static_cast<double>(b[i]);
    }

    for (size_t i = 0; i < n; ++i) {
      size_t pivotRow = i;
      double maxVal = std::abs(aug[i][i]);
      for (size_t k = i + 1; k < n; ++k) {
        if (std::abs(aug[k][i]) > maxVal) {
          maxVal = std::abs(aug[k][i]);
          pivotRow = k;
        }
      }

      if (maxVal < 1e-12) {
        throw NON_INVERTIBLE_MATRIX("Matrix is singular; system does not have a unique solution");
      }

      std::swap(aug[i], aug[pivotRow]);

      double pivot = aug[i][i];
      for (size_t j = 0; j <= n; ++j) {
        aug[i][j] /= pivot;
      }

      for (size_t k = 0; k < n; ++k) {
        if (k != i) {
          double factor = aug[k][i];
          for (size_t j = 0; j <= n; ++j) {
            aug[k][j] -= factor * aug[i][j];
          }
        }
      }
    }

    std::vector<double> x(n);
    for (size_t i = 0; i < n; ++i) {
      x[i] = aug[i][n];
    }
    return VectorND<double>(x);
  }

  // Eigenvalues and Eigenvectors
  std::vector<std::vector<T>> eigenVectors(std::vector<T>& eigenValues) const {
    if (_m != _n) {
      throw std::invalid_argument("Eigenvalues only exist for square matrices");
    }

    size_t n = _m;
    eigenValues.clear();
    std::vector<std::vector<T>> evecs;

    if (n == 0) {
      return evecs;
    }

    if (n == 1) {
      eigenValues.push_back(elements[0][0]);
      evecs.push_back({ static_cast<T>(1) });
      return evecs;
    }

    if (n == 2) {
      double a = static_cast<double>(elements[0][0]);
      double b = static_cast<double>(elements[0][1]);
      double c = static_cast<double>(elements[1][0]);
      double d = static_cast<double>(elements[1][1]);

      double tr = a + d;
      double detV = a * d - b * c;
      double disc = tr * tr - 4.0 * detV;

      if (disc < 0.0) {
        disc = 0.0;
      }

      double l1 = (tr + std::sqrt(disc)) / 2.0;
      double l2 = (tr - std::sqrt(disc)) / 2.0;

      eigenValues.push_back(static_cast<T>(l1));
      eigenValues.push_back(static_cast<T>(l2));

      double lambdas[2] = { l1, l2 };
      for (double lam : lambdas) {
        std::vector<T> v(2);
        if (std::abs(b) > 1e-9) {
          double v0 = b;
          double v1 = lam - a;
          double len = std::sqrt(v0 * v0 + v1 * v1);
          v[0] = static_cast<T>(v0 / len);
          v[1] = static_cast<T>(v1 / len);
        } else if (std::abs(c) > 1e-9) {
          double v0 = lam - d;
          double v1 = c;
          double len = std::sqrt(v0 * v0 + v1 * v1);
          v[0] = static_cast<T>(v0 / len);
          v[1] = static_cast<T>(v1 / len);
        } else {
          if (std::abs(lam - a) < std::abs(lam - d)) {
            v[0] = static_cast<T>(1);
            v[1] = static_cast<T>(0);
          } else {
            v[0] = static_cast<T>(0);
            v[1] = static_cast<T>(1);
          }
        }
        evecs.push_back(v);
      }
      return evecs;
    }

    // General n >= 3: Check if symmetric
    bool symmetric = true;
    for (size_t i = 0; i < n && symmetric; ++i) {
      for (size_t j = i + 1; j < n; ++j) {
        if (std::abs(static_cast<double>(elements[i][j]) - static_cast<double>(elements[j][i])) > 1e-9) {
          symmetric = false;
        }
      }
    }

    if (symmetric) {
      // Jacobi method for symmetric matrices
      std::vector<std::vector<double>> D(n, std::vector<double>(n));
      std::vector<std::vector<double>> V(n, std::vector<double>(n, 0.0));
      for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
          D[i][j] = static_cast<double>(elements[i][j]);
        }
        V[i][i] = 1.0;
      }

      for (size_t sweep = 0; sweep < 100; ++sweep) {
        size_t p = 0, q = 1;
        double maxOff = 0.0;
        for (size_t i = 0; i < n; ++i) {
          for (size_t j = i + 1; j < n; ++j) {
            if (std::abs(D[i][j]) > maxOff) {
              maxOff = std::abs(D[i][j]);
              p = i;
              q = j;
            }
          }
        }
        if (maxOff < 1e-12) break;

        double diff = D[q][q] - D[p][p];
        double t;
        if (std::abs(D[p][q]) < 1e-14) {
          t = 0.0;
        } else {
          double phi = diff / (2.0 * D[p][q]);
          t = 1.0 / (std::abs(phi) + std::sqrt(phi * phi + 1.0));
          if (phi < 0.0) t = -t;
        }
        double c = 1.0 / std::sqrt(t * t + 1.0);
        double s = t * c;
        double tau = s / (1.0 + c);

        double Dpq = D[p][q];
        D[p][q] = 0.0;
        D[q][p] = 0.0;
        D[p][p] -= t * Dpq;
        D[q][q] += t * Dpq;

        for (size_t r = 0; r < n; ++r) {
          if (r != p && r != q) {
            double Drp = D[r][p];
            double Drq = D[r][q];
            D[r][p] = Drp - s * (Drq + tau * Drp);
            D[p][r] = D[r][p];
            D[r][q] = Drq + s * (Drp - tau * Drq);
            D[q][r] = D[r][q];
          }
        }
        for (size_t r = 0; r < n; ++r) {
          double Vrp = V[r][p];
          double Vrq = V[r][q];
          V[r][p] = Vrp - s * (Vrq + tau * Vrp);
          V[r][q] = Vrq + s * (Vrp - tau * Vrq);
        }
      }

      for (size_t j = 0; j < n; ++j) {
        eigenValues.push_back(static_cast<T>(D[j][j]));
        std::vector<T> vec(n);
        for (size_t i = 0; i < n; ++i) {
          vec[i] = static_cast<T>(V[i][j]);
        }
        evecs.push_back(vec);
      }
      return evecs;
    }

    // QR algorithm for non-symmetric matrices
    std::vector<std::vector<double>> Ak(n, std::vector<double>(n));
    for (size_t i = 0; i < n; ++i) {
      for (size_t j = 0; j < n; ++j) {
        Ak[i][j] = static_cast<double>(elements[i][j]);
      }
    }

    for (size_t iter = 0; iter < 100; ++iter) {
      std::vector<std::vector<double>> Q(n, std::vector<double>(n, 0.0));
      std::vector<std::vector<double>> R(n, std::vector<double>(n, 0.0));
      for (size_t j = 0; j < n; ++j) {
        std::vector<double> v(n);
        for (size_t i = 0; i < n; ++i) v[i] = Ak[i][j];
        for (size_t i = 0; i < j; ++i) {
          double dot = 0.0;
          for (size_t k = 0; k < n; ++k) dot += Q[k][i] * Ak[k][j];
          R[i][j] = dot;
          for (size_t k = 0; k < n; ++k) v[k] -= dot * Q[k][i];
        }
        double norm = 0.0;
        for (size_t k = 0; k < n; ++k) norm += v[k] * v[k];
        norm = std::sqrt(norm);
        R[j][j] = norm;
        if (norm > 1e-12) {
          for (size_t k = 0; k < n; ++k) Q[k][j] = v[k] / norm;
        }
      }

      std::vector<std::vector<double>> nextA(n, std::vector<double>(n, 0.0));
      for (size_t i = 0; i < n; ++i) {
        for (size_t k = 0; k < n; ++k) {
          for (size_t j = 0; j < n; ++j) {
            nextA[i][j] += R[i][k] * Q[k][j];
          }
        }
      }
      Ak = std::move(nextA);
    }

    for (size_t i = 0; i < n; ++i) {
      eigenValues.push_back(static_cast<T>(Ak[i][i]));
    }

    for (size_t i = 0; i < n; ++i) {
      double lam = static_cast<double>(eigenValues[i]);
      Matrix<double> shifted(n, n);
      for (size_t r = 0; r < n; ++r) {
        for (size_t c = 0; c < n; ++c) {
          shifted[r][c] = static_cast<double>(elements[r][c]) - (r == c ? lam : 0.0);
        }
      }
      auto ns = shifted.nullSpace();
      std::vector<T> vec(n, static_cast<T>(0));
      if (!ns.empty()) {
        double len = 0.0;
        for (double val : ns[0]) len += val * val;
        len = std::sqrt(len);
        if (len > 1e-12) {
          for (size_t k = 0; k < n; ++k) vec[k] = static_cast<T>(ns[0][k] / len);
        }
      } else {
        vec[i] = static_cast<T>(1);
      }
      evecs.push_back(vec);
    }

    return evecs;
  }

  // Singular Value Decomposition A = U * S * V^T
  void svd(Matrix<double>& U_out, std::vector<double>& S_out, Matrix<double>& V_out) const {
    size_t m = _m;
    size_t n = _n;

    Matrix<double> A_dbl(m, n);
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        A_dbl[i][j] = static_cast<double>(elements[i][j]);
      }
    }

    Matrix<double> At = A_dbl.transpose();
    Matrix<double> AtA = At * A_dbl;

    std::vector<double> eigenVals;
    auto eigenVecs = AtA.eigenVectors(eigenVals);

    std::vector<size_t> idx(n);
    for (size_t i = 0; i < n; ++i) idx[i] = i;
    std::sort(idx.begin(), idx.end(), [&](size_t a, size_t b) {
      return eigenVals[a] > eigenVals[b];
    });

    size_t minDim = std::min(m, n);
    S_out.resize(minDim);
    V_out = Matrix<double>(n, n);
    for (size_t j = 0; j < n; ++j) {
      size_t col = idx[j];
      double sv = std::sqrt(std::max(0.0, eigenVals[col]));
      if (j < minDim) S_out[j] = sv;
      for (size_t i = 0; i < n; ++i) {
        V_out[i][j] = eigenVecs[col][i];
      }
    }

    U_out = Matrix<double>(m, m, 0.0);
    for (size_t j = 0; j < minDim; ++j) {
      if (S_out[j] > 1e-12) {
        for (size_t i = 0; i < m; ++i) {
          double sum = 0.0;
          for (size_t k = 0; k < n; ++k) {
            sum += A_dbl[i][k] * V_out[k][j];
          }
          U_out[i][j] = sum / S_out[j];
        }
      }
    }

    for (size_t j = 0; j < m; ++j) {
      double norm = 0.0;
      for (size_t i = 0; i < m; ++i) norm += U_out[i][j] * U_out[i][j];
      if (norm < 1e-6) {
        std::vector<double> e(m, 0.0);
        e[j] = 1.0;
        for (size_t k = 0; k < j; ++k) {
          double dot = 0.0;
          for (size_t i = 0; i < m; ++i) dot += U_out[i][k] * e[i];
          for (size_t i = 0; i < m; ++i) e[i] -= dot * U_out[i][k];
        }
        double enorm = 0.0;
        for (size_t i = 0; i < m; ++i) enorm += e[i] * e[i];
        enorm = std::sqrt(enorm);
        if (enorm > 1e-12) {
          for (size_t i = 0; i < m; ++i) U_out[i][j] = e[i] / enorm;
        }
      }
    }
  }

  // Print
  void print() const {
    for (size_t i = 0; i < _m; ++i) {
      std::cout << "[ ";
      for (size_t j = 0; j < _n; ++j) {
        std::cout << elements[i][j] << " ";
      }
      std::cout << "]\n";
    }
  }

  friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& mat) {
    for (size_t i = 0; i < mat.getRowDim(); ++i) {
      os << "[ ";
      for (size_t j = 0; j < mat.getColDim(); ++j) {
        os << mat[i][j] << " ";
      }
      os << "]\n";
    }
    return os;
  }
};

#endif
