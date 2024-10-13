#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
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


  double length();
  void add(VectorND<T>v);
  void transform();
  void rotateX();
  void rotateY();
  double angleWithVec(VectorND<T>v);
  bool isUnit();
  bool isZero();

  void print() const;
  std::vector<double> angleWithAxis() const; //returns array containing angles for each axis

  //for 2D and 3D vectors
  double angleWithX();
  double anglewithY();
  double anglewithZ();

  static double angleBetweenVec(VectorND<T>v1 , VectorND<T>v2);
  static VectorND<T> addVec(VectorND<T>v1 , VectorND<T>v2);
  double dotProduct(VectorND<T>v) const;
  static VectorND<T> transformVec(VectorND<T>v); 
  
};

#endif
