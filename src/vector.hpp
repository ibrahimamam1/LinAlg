#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// N-Dimensional Vector
template <typename T> class VectorND {
  size_t dimension;
  std::vector<T> components;

public:
  VectorND() : dimension(1), components(1) {}
  VectorND(size_t _dimension) : dimension(_dimension), components(dimension) {}
  VectorND(std::vector<T> vec) : dimension(vec.size()), components(vec) {}
  VectorND(VectorND<T> &v) : dimension(v.dimension), components(v.components) {}

  VectorND<T> operator=(VectorND<T> &v) {
    size_t dim = v.dimension;
    std::vector<T> comps = v.components;

    return VectorND<T>(dim, comps);
  }

  VectorND<T> operator=(std::vector<T> v) { return VectorND<T>(v.size(), v); }

  const T &operator[](size_t i) const { return components[i]; }

  size_t getDimension() const { return dimension; }
  std::vector<T> getComponents() const { return components; }

  void setComponents(std::vector<T> &_comps) {
    for (size_t i = 0; i < _comps.size() && i < dimension; i++) {
      components[i] = _comps[i];
    }
    size_t start = 0, end = 0;
    if (_comps.size() < dimension) {
      for (size_t i = _comps.size(); i < dimension; i++) {
        components[i] = 0;
      }
    }
  }

  void scale(T x) {
    for (size_t i = 0; i < this->getDimension(); i++) {
      this->components[i] *= x;
    }
  }

  // return the length of the vector
  double length() {
    long long sum_of_squares = 0;

    for (size_t i = 0; i < getDimension(); i++) {
      sum_of_squares += components[i] * components[i];
    }

    return sqrt(sum_of_squares);
  }

  // add the components of vector v to this vector object
  void add(VectorND<T> v) {

    if (dimension != v.getDimension()) {
      std::invalid_argument(
          "Dimensions Mismatch , can only add vectors of same dimension\n");
    }

    for (int i = 0; i < dimension; i++) {
      components[i] += v[i];
    }
  }

  // returns the dot product of this object with vector v
  double dotProduct(const VectorND<T> v) {
    if (dimension != v.getDimension()) {
      std::invalid_argument("Dimension Mismatch , can only find dot product if "
                            "vectors have same Dimensions\n");
    }

    double result = 0;

    for (int i = 0; i < dimension; i++) {
      result += components[i] * v[i];
    }

    return result;
  }

  // returns the angle the calling object make with the vector passes as
  // argument in degrees
  double angleWithVec(VectorND<T> v) {
    double dp = this->dotProduct(v);
    double length_u = this->length();
    double length_v = v.length();

    double angle = acos(dp / (length_u * length_v));

    return angle;
  }

  // returns true if object is a unit vector
  bool isUnit() { return this->length() == 1; }

  // return true if object is a zero vector
  bool isZero() { return this->length() == 0; }

  // prints component of the vector in human readable form
  void print() const {
    std::cout << "[ ";

    for (int i = 0; i < dimension; i++) {
      std::cout << components[i] << " ";
    }

    std::cout << "]\n";
  }
};

// child class of VectorNd for 2D vectors
template <typename T> class Vector2D : public VectorND<T> {
public:
  Vector2D() : VectorND<T>(2) {}
  Vector2D(int &x, int &y) : VectorND<T>(2) {
    this.components[0] = x;
    this.components[1] = y;
  }

  Vector2D(std::vector<T> &_comps) : VectorND<T>(_comps) {}

  Vector2D(Vector2D<T> &v) : VectorND<T>(v) {}

  // returns the angle the vector makes with the x axis
  double angleWithX() {
    Vector2D i(1, 0);
    double dp = this->dotProduct(i);

    return arccos(dp / this->length());
  }

  double anglewithY() {
    Vector2D j(0, 1);
    double dp = this->dotProduct(j);

    return arccos(dp / this->length());
  }
};

// child class of Vector3D for 3D vectors
template <typename T> class Vector3D : public VectorND<T> {
public:
  Vector3D() : VectorND<T>(3) {}
  Vector3D(int &x, int &y, int &z) : VectorND<T>(3) {
    this.components[0] = x;
    this.components[1] = y;
    this.components[2] = z;
  }

  Vector3D(std::vector<T> &_comps) : VectorND<T>(_comps) {}

  Vector3D(Vector3D<T> &v) : VectorND<T>(v) {}

  // returns the angle the vector makes with the x axis
  double angleWithX() {
    Vector3D i(1, 0, 0);
    double dp = this->dotProduct(i);

    return arccos(dp / this->length());
  }

  double anglewithY() {
    Vector3D j(0, 1, 0);
    double dp = this->dotProduct(j);

    return arccos(dp / this->length());
  }

  double anglewithZ() {
    Vector3D k(0, 0, 1);
    double dp = this->dotProduct(k);

    return arccos(dp / this->length());
  }
};
#endif
