#include "matrix.hpp"

Matrix::Matrix()
{

}
Matrix::Matrix(int rows , int columns) : m(rows) , n(columns)
{
  elements = new double*[m];
  L = new double*[m];
  U = new double*[m];
  for(size_t i=0; i<m; i++)
  {
    elements[i] = new double[n];
    L[i] = new double[n];
    U[i] = new double[n];
  } 
}

Matrix::Matrix(int rows , int columns , double *array) : m(rows) , n(columns)
{
  elements = new double*[m];
  L = new double*[m];
  U = new double*[m];
  for(size_t i=0; i<m; i++)
  {
    elements[i] = new double[n];
    L[i] = new double[n];
    U[i] = new double[n];
  } 
  setElements(array);
}

Matrix::Matrix(Matrix& source)
{
  m = source.getRows();
  n = source.getColumns();

  elements = new double*[m];
  L = new double*[m];
  U = new double*[m];
  for(size_t i=0; i<m; i++)
  {
    elements[i] = new double[n];
    L[i] = new double[n];
    U[i] = new double[n];
  } 

  double *elements_array = source.getElements();
  setElements(elements_array);
  
}
void Matrix::setElements(double* array)
{
    for(size_t i=0; i<m; i++)
    {
        for(size_t j=0; j<n; j++)
        {
            elements[i][j] = array[(n*i)+j];
            U[i][j] = elements[i][j];
            if(i == j) L[i][j] = 1; //pivot
            else L[i][j] = 0;
        }
    }
    factoriseLU();
}

size_t Matrix::getRows() const
{
  return m;
}
size_t Matrix::getColumns() const
{
  return n;
}
double* Matrix::getElements() const
{
  double* elements_array = new double[m*n];
  size_t count = 0;
  for(size_t i=0; i<m; i++)
  {
    for(size_t j=0; j < n; j++)
    {
      elements_array[count++] = elements[i][j]; 
    }
  }

  return elements_array;
}
void Matrix::print()
{
    std::cout << "Matrix : \n";
    for(size_t i=0; i<m; i++)
    {
        for(size_t j=0; j<n; j++)
        {
            std::cout << elements[i][j] << "\t";
        }
        std::cout << "\n";
    }
    std::cout << "\nL decomposition : \n";
    for(size_t i=0; i<m; i++)
    {
        for(size_t j=0; j<n; j++)
        {
            std::cout << L[i][j] << "\t";
        }
        std::cout << "\n";
    }
    std::cout << "\nU decomposition : \n";
    for(size_t i=0; i<m; i++)
    {
        for(size_t j=0; j<n; j++)
        {
            std::cout << U[i][j] << "\t";
        }
        std::cout << "\n";
    }
}

void Matrix::factoriseLU()
{
    for (size_t i = 0; i < m; i++)
    {
        for (size_t j = i + 1; j < m; j++) // Everything below pivot should be made 0
        {  
            if (elements[j][i] != 0)
            {  
                double multiplier = U[j][i] / U[i][i]; 
                L[j][i] = multiplier;           // Store multiplier in L

                for (size_t k = 0; k < n; k++) // Update U elements
                {  
                    U[j][k] -= multiplier * U[i][k];  
                }
            }
        }
    }
}

bool Matrix::isSingular()
{
    for(size_t i=0; i<m; i++)
    {
        if(U[i][i] == 0) return true;
    }
    return false;
}

bool Matrix::isInvertible()
{
    for(size_t i=0; i<m; i++)
    {
        if(U[i][i] == 0) return false;
    }
    return true;
}
double Matrix::det()
{
    if(isSingular()) return 0;
    else 
    {
      double det = 1;
      for(size_t i=0; i<m; i++)
        det *= U[i][i];
      return det;
    }
}

Matrix Matrix:: transpose()
{
  double array[m*n];
  size_t count = 0;
  for(size_t i=0; i<n; i++)
  {
    for(size_t j=0; j<m; j++)
    {
      array[count++] = elements[j][i] ;
    }
  }
  return Matrix(m , n , array);
}
Matrix Matrix::cofactorMatrix()
{
  double cofactor_array[m*n];
  size_t count = 0;
  for(size_t i=0; i<m; i++)
  {
    for(size_t j=0; j<m; j++)
    {
      //form inner Matrix
      double sub_array[(m-1) * (n-1)];
      size_t c = 0;
      for(size_t k=0; k<m; k++)
      {
        if(k == i) continue;
        for(size_t l=0; l<n; l++)
        {
          if(l == j ) continue;
          sub_array[c++] = elements[k][l];
        }
      }
      Matrix sub_matrix(m-1 , n-1 , sub_array);
      int sign;
      count % 2 == 0 ? sign = 1 : sign = -1;
      cofactor_array[count++] = sub_matrix.det() * sign;
    }
  }
  return Matrix(m , n , cofactor_array); 
}
Matrix Matrix::adjoint()
{
  Matrix cofactor = cofactorMatrix();
  double adjoint_array[m*n]; //transpose of cofactor array
  size_t count = 0;

  for(size_t i=0; i<n; i++)
  {
    for(size_t j=0; j<m; j++)
    {
      adjoint_array[count++] = cofactor.elements[j][i];
    }
  }
  return Matrix(m , n , adjoint_array);
}
Matrix Matrix::inverse()
{
  if(!isSingular()) throw NON_INVERTIBLE_MATRIX();
  Matrix adjoint_matrix = adjoint();
  double determinant = det();
  size_t count = 0;

  double inverse_array[m*n];
  for(size_t i=0; i<m; i++)
  {
    for(size_t j=0; j<n; j++)
    {
      inverse_array[count++] = adjoint_matrix.elements[i][j] / determinant;
    }
  }
  return Matrix(m , n , inverse_array);
}

Matrix Matrix::scale(double scalar)
{
  double result_array[m*n];
  size_t count = 0;

  for(size_t i=0; i<m; i++)
  {
    for(size_t j=0; j<n; j++)
    {
      result_array[count++] = elements[i][j] * scalar;
    }
  }
  return Matrix(m , n , result_array) ;
}


Vector Matrix::transform(Vector x)
{
  double result[x.getDimension()];
  for(size_t i=0; i<x.getDimension(); i++) result[i] = 0;
  size_t count = 0;

  for(size_t i=0; i<m; i++)
  {
    for(size_t j=0; j<n; j++)
    {
      result[count] += elements[i][j] * x[j];
    }
    count++;
  }
  return Vector(m , result);
}

size_t Matrix::rank()
{
  size_t rank = 0;
  for(size_t i=0; (i <m && i < n); i++)
  {
    if(U[i][i] != 0) rank++;
  }
  return rank;
}

Matrix Matrix::add(Matrix x , Matrix y)
{
  if(x.getRows() != y.getRows() || x.getColumns() != y.getColumns()) throw NON_MATCHING_MATRIX_SIZE();

  double result_array[x.getRows() * x.getColumns()];
  size_t count = 0;
  for(size_t i=0; i<x.getRows(); i++)
  {
    for(size_t j=0; j<x.getColumns(); j++)
    {
      result_array[count++] = x.elements[i][j] + y.elements[i][j];
    }
  }
  return Matrix(x.getRows() , x.getColumns() , result_array);
}

Matrix Matrix::subract(Matrix x , Matrix y)
{
  if(x.getRows() != y.getRows() || x.getColumns() != y.getColumns()) throw NON_MATCHING_MATRIX_SIZE();

  double result_array[x.getRows() * x.getColumns()];
  size_t count = 0;
  for(size_t i=0; i<x.getRows(); i++)
  {
    for(size_t j=0; j<x.getColumns(); j++)
    {
      result_array[count++] = x.elements[i][j] - y.elements[i][j];
    }
  }
  return Matrix(x.getRows() , x.getColumns() , result_array);
}

Matrix Matrix::multiply(Matrix x , Matrix y)
{
  if(x.getRows() != y.getColumns()) throw NON_MATCHING_MATRIX_SIZE();

  size_t rows = x.getRows();
  size_t columns = y.getColumns();
  double result_array[rows * columns];
  size_t count = 0;

  for(size_t i=0; i<rows * columns; i++) result_array[i] = 0; 
  
  for(size_t i=0; i<rows; i++)
  {
    for(size_t j=0; j<columns; j++)
    {
      for(size_t k=0; k < y.getRows(); k++)
      {
        result_array[count] += x.elements[i][k] * y.elements[k][j];
      }
      count++;
    }
    
  }
  return Matrix(rows , columns , result_array);
}