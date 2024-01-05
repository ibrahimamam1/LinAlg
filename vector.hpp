#ifndef VECTOR_H
#define VECTOR_H

#include<iostream>
#include<cstring>

class Vector{
  size_t dimension; //2D , 3D etc...
  double* components;

  public:

  Vector(size_t _dimension);
  Vector(size_t _dimension , double* _comps); //second argument can be array containing components
  Vector(size_t _dimension , int* _comps); 

  class Operation_With_Unmatched_Dimension; // exception class

  void setComponents(double*);
  void setComponents(int*);
  double getComponent(int ) const;
  double operator [](int i) const;
  size_t getDimension() const;
  void print() const;
  double dotProduct()const;
  double computeLength() const;
  double* computeAngles()const; //returns array containing angles for each axis in the respective order
  void scale(double scalar);
  void scale(int scalar);
  bool isUnit() const;
  bool isZero() const;


  static Vector add(Vector , Vector);
  static double dotProduct(Vector , Vector);
  static double angleBetween(Vector , Vector);
    
};


#endif 
