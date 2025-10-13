#ifndef MATRIX_H
#define MATRIX_H
#include "vector.h"

template <typename T> class Matrix {
private:
  size_t m; // Number of Rows
  size_t n; // Number of Columns
  double **elements;
  double **L;                        // L decomposition
  double **U;                        // U decomposition
  class NON_INVERTIBLE_MATRIX {};    // Exception class
  class NON_MATCHING_MATRIX_SIZE {}; // Exception class

public:
  Matrix() {}
  Matrix(size_t rows, size_t cols) : m(rows), n(cols) {
    elements = new double *[m];
    L = new double *[m];
    U = new double *[m];

    for (size_t i = 0; i < m; i++) {
      elements[i] = new double[n];
      L[i] = new double[n];
      U[i] = new double[n];
    }
  }
  Matrix(size_t rows, size_t cols, double *array) : m(rows), n(cols) {
    elements = new double* [m];
    L = new double* [m];
    U = new double* [m];
    
    for (size_t i = 0; i < m; i++) {
      elements[i] = new double[n];
      L[i] = new double[n];
      U[i] = new double[n];
    }
    setElements(array);
  }
  Matrix(Matrix<T> &); // copy constructor
  void setElements(double *array);

  size_t getRows() const;
  size_t getColumns() const;
  double *getElements() const;
  void factoriseLU();

  bool isSingular();
  bool isInvertible();
  double det(); // compute determinant

  Matrix transpose();
  Matrix cofactorMatrix();
  Matrix adjoint();
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
