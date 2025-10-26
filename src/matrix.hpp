#ifndef MATRIX_H
#define MATRIX_H
#include "vector.hpp"

template <typename T> class Matrix {
private:
  size_t m; // Number of Rows
  size_t n; // Number of Columns
  std::vector<std::vector<T>> elements;
  std::vector<std::vector<T>>L;                        // L decomposition
  std::vector<std::vector<T>>U;                        // L decomposition
  class NON_INVERTIBLE_MATRIX {};    // Exception class
  class NON_MATCHING_MATRIX_SIZE {}; // Exception class

public:
  Matrix() = delete;

  Matrix(size_t rows, size_t cols) : 
    m(rows),
    n(cols),
    elements(rows, std::vector<T>(cols, 0)),
    L(rows, std::vector<T>(cols, 0)),
    U(rows, std::vector<T>(cols, 0)) {}

  Matrix(const std::vector<std::vector<double>>& values) : 
    m(values.size()), 
    n(values[0].size()),
    elements(values),
    L(values.size(), std::vector<T>(values[0].size(), 0)),
    U(values.size(), std::vector<T>(values[0].size(), 0)) {}

  Matrix(const Matrix<T> &) = default;
  Matrix<T>& operator=(const Matrix<T> &) = default;
  
  Matrix<T>& operator=(const std::vector<std::vector<T>> &values){
    this->m = values.size();
    this->n = values[0].size();
    this->elements = values;
    return *this;
  }
  
  
  void setElements(double *array);

  size_t getRows() const;
  size_t getColumns() const;
  double *getElements() const;
  void factoriseLU();

  bool isSingular();
  bool isInvertible();
  double det(); // compute determinant

  Matrix transpose();
  Matrix inverse();
  Matrix scale(double);

  template <typename V> VectorND<V> transform(VectorND<V> x);
  size_t rank();
  void print();

  static Matrix<T> add(Matrix<T>, Matrix<T>);
  static Matrix<T> subract(Matrix<T>, Matrix<T>);
  static Matrix<T> multiply(Matrix<T>, Matrix<T>);
};

#endif
