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

  void operator=(const VectorND<T> &v) {
    this->dimension = v.dimension;
    this->components = v.components;
  }

  void operator=(const std::vector<T> &v) {
    this->dimension = v.size();
    this->components = v;
  }

  const T &operator[](size_t i) const {
    if( i >= dimension || i < 0 ) {
      throw std::out_of_range("Index out of range\n");
    }
    return components[i]; }

  size_t getDimension() const { return dimension; }
  std::vector<T> getComponents() const { return components; }

  void setComponents(const std::vector<T> &_comps) {
    for (size_t i = 0; i < _comps.size() && i < dimension; i++) {
      components[i] = _comps[i];
    }
    if (_comps.size() < dimension) {
      for (size_t i = _comps.size(); i < dimension; i++) {
        components[i] = 0;
      }
    }
  }
  template <typename U>
  auto scale(const U x) {
    using ResultType = decltype(components[0] * x);
    std::vector<ResultType> new_comps(this->dimension);
    for (size_t i = 0; i < this->getDimension(); i++) {
       new_comps[i] = this->components[i] * x;
    }
    return VectorND<ResultType>(new_comps);
  }

  // return the length of the vector
  double length() const{
    double sum_of_squares = 0;

    for (size_t i = 0; i < getDimension(); i++) {
      sum_of_squares += components[i] * components[i];
    }

    return sqrt(sum_of_squares);
  }
  
  // add the components of vector v to this vector object
  template <typename U>
  auto operator +(const VectorND<U>& v) {
    if (dimension != v.getDimension()) {
     throw std::invalid_argument(
          "Dimensions Mismatch , can only add vectors of same dimension\n");
    }

    using Type = decltype(components[0] + v[0]);
    std::vector<Type> new_comps(this->dimension);
    
    for (size_t i = 0; i < dimension; i++) {
      new_comps[i] = components[i] + v[i];
    }
    return VectorND<Type>(new_comps);
  }
 
  // subtract the components of vector v from this vector object
  template <typename U>
  auto operator -(const VectorND<U>& v) {
    if (dimension != v.getDimension()) {
     throw std::invalid_argument(
          "Dimensions Mismatch , can only subtract vectors of same dimension\n");
    }

    using Type = decltype(components[0] + v[0]);
    std::vector<Type> new_comps(this->dimension);
    
    for (size_t i = 0; i < dimension; i++) {
      new_comps[i] = components[i] - v[i];
    }
    return VectorND<Type>(new_comps);
  }
  
  template <typename U>
  VectorND<T>& operator +=(const VectorND<U> &v) {
    if (dimension != v.getDimension()) {
     throw std::invalid_argument(
          "Dimensions Mismatch , can only add vectors of same dimension\n");
    }

    for (size_t i = 0; i < dimension; i++) {
      components[i] += v[i];
    }
    return *this;
  }

  template <typename U>
  VectorND<T>& operator -=(const VectorND<U> &v) {
    if (dimension != v.getDimension()) {
     throw std::invalid_argument(
          "Dimensions Mismatch , can only subtract vectors of same dimension\n");
    }

    for (size_t i = 0; i < dimension; i++) {
      components[i] -= v[i];
    }
    return *this;
  }
  
  template <typename U>
  auto add(const VectorND<U>& v) {
    if (dimension != v.getDimension()) {
     throw std::invalid_argument(
          "Dimensions Mismatch , can only add vectors of same dimension\n");
    }

    using Type = decltype(components[0] + v[0]);
    std::vector<Type> new_comps(this->dimension);
    
    for (size_t i = 0; i < dimension; i++) {
      new_comps[i] = components[i] + v[i];
    }
    return VectorND<Type>(new_comps);
  }
 
  // subtract the components of vector v from this vector object
  template <typename U>
  auto subtract(const VectorND<U>& v) {
    if (dimension != v.getDimension()) {
      throw std::invalid_argument(
          "Dimensions Mismatch , can only subtract vectors of same dimension\n");
    }

    using Type = decltype(components[0] - v[0]);
    std::vector<Type> new_comps(this->dimension);
    
    for (size_t i = 0; i < dimension; i++) {
      new_comps[i] = components[i] - v[i];
    }
    return VectorND<Type>(new_comps);
  }


  // returns the dot product of this object with vector v
  template <typename U>
  double dotProduct(const VectorND<U> &v) {
    if (dimension != v.getDimension()) {
      throw std::invalid_argument("Dimension Mismatch , can only find dot product if "
                            "vectors have same Dimensions\n");
    }

    double result = 0;

    for (int i = 0; i < dimension; i++) {
      result += components[i] * v[i];
    }

    return result;
  }

  // returns the angle the calling object make with the vector passed as in degrees
  template <typename U>
  double angleWithVec(const VectorND<U> &v) {
    double dp = this->dotProduct(v);
    double length_u = this->length();
    double length_v = v.length();

    double cosine = dp / (length_u * length_v);
    cosine = std::max(-1.0, std::min(1.0, cosine));
    double angle = acos(cosine);

    return angle;
  }

  // returns true if object is a unit vector
  bool isUnit() {
   double length = this->length();
   return std::abs(length - 1.0) < 1e-6;
  }

  // return true if object is a zero vector
  bool isZero() { return this->length() == 0; }

  // prints component of the vector in human readable form
  void print() const {
    std::cout << "[ ";

    for (size_t i = 0; i < dimension; i++) {
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
    return this->angleWithVec(i);
  }

  double anglewithY() {
    Vector2D j(0, 1);
    return this->angleWithVec(j);
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
    return this->angleWithVec(i);
  }

  double anglewithY() {
    Vector3D j(0, 1, 0);
    return this->angleWithVec(j);
  }

  double anglewithZ() {
    Vector3D k(0, 0, 1);
    return this->angleWithVec(k);
  }
};
#endif
