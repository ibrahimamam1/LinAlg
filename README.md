# LinALG

**LinALG** is a modern, lightweight, header-only C++17 linear algebra library. It provides high-performance, templated implementations for vectors and matrices, featuring intuitive overloaded operator arithmetic, robust matrix factorizations (PLU, SVD), subspace analysis, eigenvalue/eigenvector computation, and linear system solvers.

---

## Table of Contents

- [Features](#features)
- [Requirements](#requirements)
- [Installation & Integration](#installation--integration)
- [Building & Running Tests](#building--running-tests)
- [Quick Start Guide](#quick-start-guide)
  - [1. Vector Operations](#1-vector-operations)
  - [2. Matrix Construction & Basics](#2-matrix-construction--basics)
  - [3. Overloaded Operators (Arithmetic)](#3-overloaded-operators-arithmetic)
  - [4. Determinant & Inversion](#4-determinant--inversion)
  - [5. LU Factorization (with Pivoting)](#5-lu-factorization-with-pivoting)
  - [6. Linear System Solving ($Ax = b$)](#6-linear-system-solving-ax--b)
  - [7. Rank and Fundamental Subspaces](#7-rank-and-fundamental-subspaces)
  - [8. Eigenvalues & Eigenvectors](#8-eigenvalues--eigenvectors)
  - [9. Singular Value Decomposition (SVD)](#9-singular-value-decomposition-svd)
- [API Reference](#api-reference)
  - [`VectorND<T>` / `Vector2D<T>` / `Vector3D<T>`](#vectorndt--vector2dt--vector3dt)
  - [`Matrix<T>`](#matrixt)

---

## Features

- **Header-Only**: Zero external library dependencies beyond the C++ standard library. Drop into your project and go.
- **Templated & Type-Safe**: Supports `double`, `float`, `int`, and custom arithmetic types with implicit type promotion across operations.
- **Intuitive Overloaded Operators**:
  - Matrix-matrix addition (`A + B`), subtraction (`A - B`), and multiplication (`A * B`).
  - Scalar arithmetic (`A * s`, `s * A`), unary negation (`-A`).
  - Matrix-vector multiplication (`A * v` or `A.transform(v)`).
  - Compound assignment (`+=`, `-=`, `*=`).
  - Equality checks (`==`, `!=`) with floating-point tolerance handling.
  - Stream insertion (`std::cout << mat`).
- **Decompositions & Solvers**:
  - **PLU Factorization**: Doolittle algorithm with partial row pivoting for numerical stability ($P A = L U$).
  - **Gauss-Jordan Inversion**: Numerically stable matrix inverse with singularity detection.
  - **Direct Linear Solver**: Exact solutions to $A x = b$ using augmented Gauss-Jordan elimination.
  - **Eigen Decomposition**: Analytical solutions for $2 \times 2$, Jacobi method for symmetric matrices, and QR iteration for general square matrices.
  - **Singular Value Decomposition (SVD)**: $A = U \Sigma V^T$ for general rectangular matrices.
- **Subspace Analysis**:
  - Exact matrix **rank** via Reduced Row Echelon Form (RREF).
  - Basis extraction for **Column Space**, **Row Space**, and **Null Space** (Kernel).

---

## Requirements

- **C++ Standard**: C++17 or newer.
- **Compiler**: GCC 7+, Clang 5+, or MSVC 2017+.
- **Build System**: CMake 3.20+ (for building unit tests).
- **Testing Framework**: GoogleTest (automatically detected via CMake for test runner).

---

## Installation & Integration

LinALG is header-only. Simply copy or symlink the `src/` directory into your project include path:

```
LinAlg/
└── src/
    ├── vector.hpp
    └── matrix.hpp
```

### In Your C++ Files
```cpp
#include "matrix.hpp"
#include "vector.hpp"
```

### In `CMakeLists.txt`
```cmake
target_include_directories(your_target_name PRIVATE /path/to/LinAlg/src)
```

---

## Building & Running Tests

Unit tests are written with GoogleTest. To configure, build, and execute the tests:

```bash
# 1. Create and enter build directory
mkdir build && cd build

# 2. Configure with CMake
cmake ../test

# 3. Build test executables
make

# 4. Run tests with CTest or directly
ctest --output-on-failure

# Alternatively, execute the test binaries directly:
./runVectorTests
./runMatrixTests
```

---

## Quick Start Guide

### 1. Vector Operations

LinALG includes `VectorND<T>` for $N$-dimensional vectors, with specialized derived classes `Vector2D<T>` and `Vector3D<T>`.

```cpp
#include "vector.hpp"
#include <iostream>

int main() {
    // Construct vectors
    VectorND<double> u({1.0, 2.0, 3.0});
    VectorND<double> v({4.0, 5.0, 6.0});

    // Vector arithmetic via operators
    auto sum = u + v;          // [5, 7, 9]
    auto diff = v - u;         // [3, 3, 3]
    auto scaled = u.scale(2.0); // [2, 4, 6]

    // Dot product and geometric properties
    double dot = u.dotProduct(v);        // 32.0
    double len = u.length();            // sqrt(14) ≈ 3.7416
    double angle = u.angleWithVec(v);   // Angle in radians

    // 2D and 3D vector convenience
    int x = 1, y = 0, z = 0;
    Vector3D<int> unitX(x, y, z);
    std::cout << "Angle with X axis: " << unitX.angleWithX() << " rad\n";

    return 0;
}
```

### 2. Matrix Construction & Basics

```cpp
#include "matrix.hpp"
#include <iostream>

int main() {
    // 1. Construct from dimensions (rows, cols, optional initial value)
    Matrix<double> A(2, 3, 0.0);

    // 2. Construct from 2D std::vector
    Matrix<double> B({
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0}
    });

    // 3. Factory methods
    auto I = Matrix<double>::identity(3);      // 3x3 Identity matrix
    auto Z = Matrix<double>::zeros(2, 4);         // 2x4 Zero matrix
    auto O = Matrix<double>::ones(3, 3);          // 3x3 Matrix of ones
    auto D = Matrix<double>::diag({2.0, 4.0, 6.0}); // 3x3 Diagonal matrix

    // Element access via [] operator or .at(row, col)
    B[0][1] = 5.0;
    std::cout << "Rows: " << B.getRowDim() << ", Cols: " << B.getColDim() << "\n";
    std::cout << B << "\n";

    return 0;
}
```

### 3. Overloaded Operators (Arithmetic)

LinALG overloads operators for arithmetic expressions:

```cpp
Matrix<double> A({
    {1.0, 2.0},
    {3.0, 4.0}
});
Matrix<double> B({
    {5.0, 6.0},
    {7.0, 8.0}
});

// Matrix Addition & Subtraction
auto C = A + B;
auto D = B - A;

// Matrix Multiplication
auto Prod = A * B;

// Scalar Multiplication (commutative)
auto Scaled1 = A * 2.5;
auto Scaled2 = 2.5 * A;

// Compound Assignment
A += B;
A -= B;
A *= 2.0;

// Matrix-Vector Multiplication / Transformation
VectorND<double> v({1.0, 2.0});
VectorND<double> transformed = A * v;      // or A.transform(v)

// Stream Insertion
std::cout << Prod;
```

### 4. Determinant & Inversion

```cpp
Matrix<double> A({
    {1.0, 2.0},
    {3.0, 4.0}
});

// Determinant (cached automatically)
double det = A.det(); // -2.0

// Invertibility checks
if (A.isInvertible()) {
    Matrix<double> invA = A.inverse();
    std::cout << "Inverse:\n" << invA;

    // Verify A * A^-1 == I
    auto identity = A * invA;
    std::cout << "A * inv(A):\n" << identity;
}
```

### 5. LU Factorization (with Pivoting)

Computes the PLU decomposition $P A = L U$, where $P$ is the row permutation vector, $L$ is unit lower triangular, and $U$ is upper triangular:

```cpp
Matrix<double> M({
    {0.0, 2.0, 1.0},
    {1.0, 5.0, 3.0},
    {2.0, 1.0, 4.0}
});

M.factoriseLU();

const auto& L = M.getL();
const auto& U = M.getU();
const auto& P = M.getP(); // Permutation indices

std::cout << "L:\n" << L;
std::cout << "U:\n" << U;
```

### 6. Linear System Solving ($Ax = b$)

Solve $A x = b$ for square, non-singular systems:

```cpp
Matrix<double> A({
    {2.0, 1.0},
    {1.0, 3.0}
});
VectorND<double> b({5.0, 5.0});

// Solve Ax = b
VectorND<double> x = A.solve(b); // x = [2.0, 1.0]
std::cout << "Solution: ";
x.print();
```

### 7. Rank and Fundamental Subspaces

Compute the matrix rank and extract explicit bases for the core fundamental subspaces:

```cpp
Matrix<double> M({
    {1.0, 2.0, 3.0},
    {2.0, 4.0, 6.0}
});

std::cout << "Rank: " << M.rank() << "\n"; // 1

// Column space basis (vectors of size m)
auto colBasis = M.columnSpace();

// Row space basis (vectors of size n)
auto rowBasis = M.rowSpace();

// Null space basis (vectors in R^n satisfying M * v = 0)
auto nullBasis = M.nullSpace();
for (const auto& basisVec : nullBasis) {
    VectorND<double> v(basisVec);
    auto zero = M * v; // zero vector
}
```

### 8. Eigenvalues & Eigenvectors

```cpp
Matrix<double> sym({
    {2.0, 1.0},
    {1.0, 2.0}
});

std::vector<double> eigenValues;
auto eigenVectors = sym.eigenVectors(eigenValues);

for (size_t i = 0; i < eigenValues.size(); ++i) {
    std::cout << "Eigenvalue: " << eigenValues[i] << ", Eigenvector: [ ";
    for (double val : eigenVectors[i]) {
        std::cout << val << " ";
    }
    std::cout << "]\n";
}
```

### 9. Singular Value Decomposition (SVD)

Decomposes an $m \times n$ matrix into $A = U \Sigma V^T$:

```cpp
Matrix<double> A({
    {3.0, 2.0,  2.0},
    {2.0, 3.0, -2.0}
});

Matrix<double> U(2, 2);
std::vector<double> S;   // Singular values sorted in descending order
Matrix<double> V(3, 3);

A.svd(U, S, V);

std::cout << "Singular values: ";
for (double s : S) std::cout << s << " ";
std::cout << "\n";
```

---

## API Reference

### `VectorND<T>` / `Vector2D<T>` / `Vector3D<T>`

| Method / Operator | Description |
|---|---|
| `VectorND()` | Default constructor (1D zero vector) |
| `VectorND(size_t dim)` | Initializes vector of dimension `dim` with zeros |
| `VectorND(std::vector<T> vec)` | Initializes vector from standard `std::vector` |
| `v[i]` | Accesses component at index `i` (with bounds checking) |
| `getDimension()` | Returns vector dimension |
| `getComponents()` | Returns underlying `std::vector<T>` |
| `setComponents(std::vector<T>)` | Updates vector components |
| `length()` | Returns Euclidean length / magnitude $\|v\|$ |
| `dotProduct(other)` | Computes scalar dot product $u \cdot v$ |
| `angleWithVec(other)` | Returns angle with `other` in radians |
| `isUnit()`, `isZero()` | Checks if vector is unit length or zero vector |
| `scale(scalar)` | Returns vector scaled by scalar value |
| `u + v`, `u - v` | Vector addition and subtraction |
| `u += v`, `u -= v` | Compound addition and subtraction |
| `print()` | Prints vector components formatted to `std::cout` |
| `Vector2D(x, y)` | 2D vector with `.angleWithX()`, `.angleWithY()` |
| `Vector3D(x, y, z)` | 3D vector with `.angleWithX()`, `.angleWithY()`, `.angleWithZ()` |

### `Matrix<T>`

| Method / Operator | Description |
|---|---|
| `Matrix(rows, cols, initVal = 0)` | Construct matrix with specified size and initial value |
| `Matrix(std::vector<std::vector<T>>)` | Construct matrix from nested 2D vector |
| `Matrix::identity(size)` | Creates $n \times n$ identity matrix |
| `Matrix::zeros(m, n)` | Creates $m \times n$ zero matrix |
| `Matrix::ones(m, n)` | Creates $m \times n$ all-ones matrix |
| `Matrix::diag(std::vector<T>)` | Creates diagonal matrix from vector of values |
| `mat[i][j]` / `mat.at(i, j)` | Access elements by row and column (bounds checked) |
| `getRowDim()`, `getColDim()` | Returns number of rows and columns |
| `setElements(values)` | Replaces matrix elements and invalidates cache |
| `to2DVector()`, `to1DVector()` | Converts matrix to nested or flattened `std::vector` |
| `A + B`, `A - B` | Matrix elementwise addition and subtraction |
| `A * B` | Matrix multiplication ($m \times k$ by $k \times p$) |
| `A * v`, `A.transform(v)` | Matrix-vector multiplication ($m \times n$ by $n \times 1$) |
| `A * s`, `s * A`, `scale(s)` | Scalar multiplication |
| `-A` | Unary negation (negates each element) |
| `A += B`, `A -= B`, `A *= B`, `A *= s` | Compound assignment operators |
| `A == B`, `A != B` | Matrix equality/inequality comparison |
| `transpose()` | Returns transpose $A^T$ |
| `det()` | Computes determinant (cached) |
| `isInvertible()`, `isSingular()` | Checks if matrix is invertible or singular |
| `factoriseLU()` / `getL()`, `getU()`, `getP()` | PLU factorization with partial pivoting ($PA = LU$) |
| `inverse()` | Computes matrix inverse $A^{-1}$ via Gauss-Jordan |
| `solve(VectorND<T> b)` | Solves linear system $Ax = b$ |
| `rank()` | Computes exact matrix rank via RREF |
| `columnSpace()`, `rowSpace()` | Returns basis for column space and row space |
| `nullSpace()` | Returns basis vectors for null space / kernel |
| `eigenVectors(eigenValues)` | Computes eigenvalues and corresponding eigenvectors |
| `svd(U, S, V)` | Singular Value Decomposition ($A = U \Sigma V^T$) |
| `print()`, `operator<<` | Pretty-prints matrix formatted with brackets |

---

