#ifndef MATRIX_HPP
#define MATRIX_HPP
#include "vector.hpp"
class Matrix
{
  private:
  size_t m; //Number of Rows
  size_t n; //Number of Columns
  double **elements;
  double **L; //L decomposition
  double **U; //U decomposition
  class NON_INVERTIBLE_MATRIX{}; //Exception class
  class NON_MATCHING_MATRIX_SIZE{}; //Exception class

  public:
  Matrix();
  Matrix(int rows , int columns);
  Matrix(int rows , int columns , double* array);
  Matrix(Matrix&); //copy constructor
  void setElements(double* array);

  size_t getRows() const;
  size_t getColumns() const;
  double* getElements() const;
  void factoriseLU();

  bool isSingular(); 
  bool isInvertible(); 
  double det(); // compute determinant

  Matrix transpose();
  Matrix cofactorMatrix();
  Matrix adjoint();
  Matrix inverse();
  Matrix scale(double );
        
  Vector transform(Vector x);
  size_t rank();
  void print();

  static Matrix add(Matrix , Matrix);
  static Matrix subract(Matrix , Matrix);
  static Matrix multiply(Matrix , Matrix);
};

#endif