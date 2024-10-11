#ifndef VECTOR_H
#define VECTOR_H

#include<cstring>
#include <memory>

//N-Dimensional Vector

template<typename T> class VectorND{
  size_t dimension; //2D , 3D etc...
  T* components; //array of the components of the vector

  public:

  VectorND(size_t _dimension) : dimension(_dimension){}
  VectorND(size_t _dimension , T* _comps) : dimension(_dimension){
    components = new T[dimension]();

    for(size_t i=0; i<dimension; i++) {
      components[i] = _comps[i];
    }
  } 

  void setComponents(T* _comps){
    components = new T[dimension]();
    
    for(size_t i=0; i<dimension; i++) {
      components[i] = _comps[i];
    } 
  }

  T* getComponent(int ) const{ return components; }
  T* operator [](int i) const{ return components[i]; }

  size_t getDimension() const { return dimension;}

  /* void print() const;
  T* dotProduct()const;
  double computeLength() const;
  double* computeAngles()const; //returns array containing angles for each axis in the respective order
  void scale(double scalar);
  void scale(int scalar);
  bool isUnit() const;
  bool isZero() const;

  static Vector add(Vector , Vector);
  static double dotProduct(Vector , Vector);
  static double angleBetween(Vector , Vector);
  */  
};


#endif 
