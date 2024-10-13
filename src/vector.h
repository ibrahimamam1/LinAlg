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
  size_t dimension;          // 2D , 3D etc...
  std::vector<T> components; // array of the components of the vector

public:
  VectorND() : dimension(0) , components(0){}
  VectorND(size_t _dimension) : dimension(_dimension), components(dimension) {}
  VectorND(size_t _dimension, std::vector<T>& _comps) : dimension(_dimension) , components(dimension){
    if(_comps.size() > dimension){
      throw std::invalid_argument("In Constructor : cannot assign components of size : " + std::to_string(_comps.size()) + " to Vector of dimension : " + std::to_string(dimension));
  }
    for (size_t i = 0; i < dimension; i++) {
      components[i] = _comps[i];
    }
  }

  VectorND(VectorND<T>& v) : dimension(v.dimension) , components(v.components){}
  VectorND<T> operator =(VectorND<T>& v){
    size_t dim = v.dimension;
    std::vector<T> comps = v.components;

    return VectorND<T>(dim , comps);
  }
  
  VectorND(std::vector<T>vec) : dimension(vec.size()) , components(vec) {}
  void setComponents(std::vector<T>& _comps) {
   if(_comps.size() > dimension){
      throw std::invalid_argument("In Constructor : cannot assign components of size : " + std::to_string(_comps.size()) + " to Vector of dimension : " + std::to_string(dimension));
    }

    for (size_t i = 0; i < dimension; i++) {
      components[i] = _comps[i];
    }
  }

  const T& operator[](int i) const { return components[i]; }
  
  std::vector<T>& getComponent(int) const { return components; }
  size_t getDimension() const { return dimension; }
  
void scale(T x)
{
    for(size_t i=0; i<this->getDimension() ; i++)
    {
        this->components[i] *= x;
    }
}


  double length(){
    long long sum_of_squares = 0;

    for(size_t i=0; i<getDimension(); i++){
      sum_of_squares += components[i] * components[i];
    }

    return sqrt(sum_of_squares);
  }

  //add the components of vector v to this vector object
  void add(VectorND<T>v) {
    
    if(dimension != v.getDimension()){
      std::invalid_argument("Dimensions Mismatch , can only add vectors of same dimension\n");
    }

    for(int i=0; i<dimension; i++){
      components[i] += v[i];
    }
  }
  
  //computes the dot product of this object with vector v
  double dotProduct(VectorND<T>v) const{
    if(dimension != v.getDimension()) {
      std::invalid_argument("Dimension Mismatch , can only find dot product if vectors have same Dimensions\n");
    }

    double result = 0;

    for(int i=0; i<dimension; i++){
      result += components[i]*v[i];
    }

    return result;
  }
  //TODO: Transform Using a matrix
  //void transform();
  double angleWithVec(VectorND<T>v){
    double dp = this->dotProduct(v);
    double length_u = this->length();
    double length_v = v.length();

    double angle = acos( dp / (length_u * length_v) );

    return angle;
  }
  bool isUnit() {
    return this->length() == 1;
  }
  bool isZero() {
    return this->length() == 0;
  }

  void print() const{
    std::cout << "[ ";

    for(int i=0; i<dimension; i++){
      std::cout << v[i] << " ";
    }

    std::cout << "]\n";
  }

  //for 2D and 3D vectors
  virtual double angleWithX();
  virtual double anglewithY();
  virtual double anglewithZ();
  virtual std::vector<double> angleWithAxis() const; //returns array containing angles for each axis
  virtual void rotateX();
  virtual void rotateY();
  virtual void rotateZ();

  //for vector.cpp
  static double angleBetweenVec(VectorND<T>v1 , VectorND<T>v2);
  static VectorND<T> addVec(VectorND<T>v1 , VectorND<T>v2);
  static double dotProduct(VectorND<T>v1 , VectorND<T>v2) const;
  static VectorND<T> transformVec(VectorND<T>v); 
  
};

#endif
