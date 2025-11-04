#ifndef MATRIX_H
#define MATRIX_H
#include "vector.hpp"

template <typename T> class Matrix {
private:
  size_t m; // Number of Rows
  size_t n; // Number of Columns
  std::vector<std::vector<T>> elements;
  std::unique_ptr<Matrix<T>> L = nullptr; // Lower triangular matrix for LU factorization
  std::unique_ptr<Matrix<T>> U = nullptr; // Upper triangular matrix for LU factorization
  mutable bool luFactorized = false;
  mutable double determinant = 0.0;
  mutable bool determinantComputed = false;
  mutable size_t rank = 0;
  mutable bool rankComputed = false;
  class NON_INVERTIBLE_MATRIX {};    // Exception class
  class NON_MATCHING_MATRIX_SIZE {}; // Exception class

public:
  Matrix() = delete;

  Matrix(size_t rows, size_t cols) : 
    m(rows),
    n(cols),
    elements(rows, std::vector<T>(cols, 0)){}

  Matrix(const std::vector<std::vector<double>>& values) : 
    m(values.size()), 
    n(values[0].size()),
    elements(values){}

  Matrix(const Matrix<T> &) = default;
  Matrix<T>& operator=(const Matrix<T> &) = default;
  
  Matrix<T>& operator=(const std::vector<std::vector<T>> &values){
    this->m = values.size();
    this->n = values[0].size();
    this->elements = values;
    return *this;
  }

  const std::vector<T>& operator[](size_t i) const {
    if (i >= m) {
      throw std::out_of_range("Index out of range\n");
    }
    return elements[i];
  }

  size_t getRows() const {
    return elements.size();
  }

  size_t getCols() const {
    return elements[0].size();
  }
  
  
  Matrix<T>& factoriseLU(Matrix<T>& U) {
      if (luFactorized) {
          U = *this->U;
          return *this->L;
      }
      
      L = std::make_unique<Matrix<T>>(m, n);
      this->U = std::make_unique<Matrix<T>>(*this); // Copy current matrix to U
      
      // Initialize L as identity
      for(size_t i = 0; i < m; ++i) {
          for(size_t j = 0; j < n; ++j) {
              L->elements[i][j] = (i == j) ? 1 : 0;
          }
      }
      
      // Perform LU decomposition (Doolittle algorithm)
      for(size_t k = 0; k < std::min(m, n); ++k) {
          if(this->U->elements[k][k] == 0) {
              throw std::runtime_error("Matrix has a zero pivot, cannot perform LU factorization\n");
          }
          
          // For each row below the pivot
          for(size_t i = k + 1; i < m; ++i) {
              // Calculate multiplier
              T factor = this->U->elements[i][k] / this->U->elements[k][k];
              L->elements[i][k] = factor;
              
              // Eliminate column k in row i
              for(size_t j = k; j < n; ++j) {
                  this->U->elements[i][j] -= factor * this->U->elements[k][j];
              }
          }
      }
      
      luFactorized = true;
      U = *this->U;  // Copy result to output parameter
      return *L;
  }

  bool isInvertible(){
    if(determinantComputed){
      return determinant != 0.0;
    }
    determinant = det();
    determinantComputed = true;
    return determinant != 0.0;  
  }

  double det(){
    if(determinantComputed){
      return determinant;
    }
    if(m != n){
      throw std::invalid_argument("Determinant only defined for square matrices");
    }
    Matrix<T> U(m, n);
    factoriseLU(U);
    double detValue = 1.0;
    for(size_t i = 0; i < n; ++i){
      detValue *= U[i][i];
    }
    determinant = detValue;
    determinantComputed = true;
    return determinant;
  }

  //Matrix transpose();
  //Matrix inverse();
  //Matrix scale(double);

  //template <typename V> VectorND<V> transform(VectorND<V> x);
  //size_t rank();
  //void print();

  // std::vector<std::vector<T>> columnSpace();
  // std::vector<std::vector<T>> rowSpace();
  // std::vector<std::vector<T>> nullSpace();

  // std::vector<std::vector<T>> to2DVector() const;
  // std::vector<T> to1DVector() const;

  //std::vector<std::vector<T>> eigenVectors(std::vector<T>& eigenValues);

  //static Matrix<T> add(Matrix<T>, Matrix<T>);
  //static Matrix<T> subract(Matrix<T>, Matrix<T>);
  //static Matrix<T> multiply(Matrix<T>, Matrix<T>);
};

#endif
