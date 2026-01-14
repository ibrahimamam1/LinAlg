#ifndef MATRIX_H
#define MATRIX_H
#include "vector.hpp"
#include <vector>

template <typename T> class Matrix {
private:
  size_t _m; // Number of Rows
  size_t _n; // Number of Columns
  std::vector<std::vector<T>> elements;
  std::unique_ptr<Matrix<T>> L =
      nullptr; // Lower triangular matrix for LU factorization
  std::unique_ptr<Matrix<T>> U =
      nullptr; // Upper triangular matrix for LU factorization
  mutable bool luFactorized = false;
  mutable double determinant = 0.0;
  mutable bool determinantComputed = false;
  mutable size_t rank = 0;
  mutable bool rankComputed = false;
  class NON_INVERTIBLE_MATRIX {};    // Exception class
  class NON_MATCHING_MATRIX_SIZE {}; // Exception class

public:
  Matrix() = delete;

  Matrix<T>(size_t m, size_t n)
      : _m(m), _n(n), elements(m, std::vector<T>(n, 0)) {}

  Matrix<T>(const std::vector<std::vector<T>> &values)
      : _m(values.size()), _n(values[0].size()), elements(values) {}

  Matrix<T>(const Matrix<T> &) = default;
  Matrix<T> &operator=(const Matrix<T> &) = default;

  Matrix<T> &operator=(const std::vector<std::vector<T>> &values) {
    this->_m = values.size();
    this->_n = values[0].size();
    this->elements = values;
    return *this;
  }

  void setElements(const std::vector<std::vector<T>>& values){
    this->elements = values;
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

  size_t getRowDim() const { return elements.size(); }
  size_t getColDim() const { return elements[0].size(); }
  
  //TODO: overloaded +, - , == ,and * operators
  //===========MARK: UNTESTED LAND STARTS HERE===============
/*  Matrix<T> &factoriseLU(Matrix<T> &U) {

 }*/

  bool isInvertible() {
    if (determinantComputed) {
      return determinant != 0.0;
    }
    //determinant = det();
    determinantComputed = true;
    return determinant != 0.0;
  }

  /*double det() {
    if (determinantComputed) {
      return determinant;
    }
    if (m != n) {
      throw std::invalid_argument(
          "Determinant only defined for square matrices");
    }
    Matrix<T> U(m, n);
    factoriseLU(U);
    double detValue = 1.0;
    for (size_t i = 0; i < n; ++i) {
      detValue *= U[i][i];
    }
    determinant = detValue;
    determinantComputed = true;
    return determinant;
  }
*/
  // Matrix transpose();
  // Matrix inverse();
  // Matrix scale(double);

  // template <typename V> VectorND<V> transform(VectorND<V> x);
  // size_t rank();
  // void print();

  // std::vector<std::vector<T>> columnSpace();
  // std::vector<std::vector<T>> rowSpace();
  // std::vector<std::vector<T>> nullSpace();

  // std::vector<std::vector<T>> to2DVector() const;
  // std::vector<T> to1DVector() const;

  // std::vector<std::vector<T>> eigenVectors(std::vector<T>& eigenValues);

  // static Matrix<T> add(Matrix<T>, Matrix<T>);
  // static Matrix<T> subract(Matrix<T>, Matrix<T>);
  // static Matrix<T> multiply(Matrix<T>, Matrix<T>);
};

#endif
