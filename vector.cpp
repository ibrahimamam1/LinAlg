#include <cstddef>
#include <cstdint>
#include<iostream>
#include<cmath>
#include <math.h>
#include"vector.hpp"

Vector::Vector(size_t _dimension) : dimension(_dimension)
{
    components = new double[dimension]();
}

Vector::Vector(size_t _dimension , double* _comps) : dimension(_dimension)
{
    components = new double[dimension]();
    for(size_t i=0; i <dimension; i++)
    {
        components[i] = _comps[i];
    }
}

Vector::Vector(size_t _dimension , int* _comps) : dimension(_dimension)
{
    components = new double[dimension]();
    for(size_t i=0; i <dimension; i++)
    {
        components[i] = _comps[i];
    }
}
class Vector::Operation_With_Unmatched_Dimension // exception class
{
  public:
  size_t dim_x; // Dimensions of vector x
  size_t dim_y; //Dimensions of vector y
  char operation_type[20];
  Operation_With_Unmatched_Dimension (int _dim_x , int _dim_y , char* _opType) : dim_x(_dim_x) , dim_y(_dim_y)
  {
    strcpy(operation_type , _opType);
  }
};

void Vector::setComponents(double* _array)
{
    for(size_t i=0; i<dimension; i++)
    {
        components[i] = _array[i];
    }
}

void Vector::setComponents(int* _array)
{
    for(size_t i=0; i<dimension; i++)
    {
        components[i] = _array[i];
    }
}

double Vector::getComponent(int i) const
{
    return components[i];
}

double Vector::operator [](int i) const
{
    return components[i];
}
size_t Vector::getDimension() const
{
    return dimension;
}
void Vector::print() const
{
    std::cout << "(";
    for(size_t i=0; i<this->getDimension(); i++)
    {
        std::cout << this->components[i]; 
        if(i != getDimension()-1) std::cout << " , "; 
    }
    std::cout << ")\n";
        
}

double Vector::dotProduct() const
{
    double result = 0;
    for(size_t i=0; i<this->getDimension(); i++)
        result += this->components[i]*this->components[i];

    return result;

}

double Vector::computeLength() const
{
    return sqrt(this->dotProduct());
}
double* Vector::computeAngles()const
{
    double* result = new double(this->getDimension());
    int length = this->computeLength();

    for(size_t i=0; i < this->getDimension(); i++)
    {
        result[i] = acos(this->getComponent(i) / length);
    } 

    return result;
}

void Vector::scale(double scalar)
{
    for(size_t i=0; i<this->getDimension() ; i++)
    {
        this->components[i] *= scalar;
    }
}
void Vector::scale(int scalar)
{
    for(size_t i=0; i<this->getDimension() ; i++)
    {
        this->components[i] *= scalar;
    }
}

bool Vector::isUnit() const
{
    if(this->computeLength() == 1) return true;
    return false;
}

bool Vector::isZero() const
{
    if(this->computeLength() == 0) return true;
    return false;
}

Vector Vector::add(Vector x, Vector y)
{
    if(x.getDimension() != y.getDimension())
    {
        char opType[] = "Addition";
        throw Operation_With_Unmatched_Dimension(x.getDimension() , y.getDimension() , opType);
    }
        
    else 
    {
        double* result = new double[x.getDimension()];
        for(size_t i=0; i<x.getDimension(); i++)
        {
            result[i] = x.components[i] + y.components[i]; 
        }

        return Vector(x.getDimension() , result);
    }
}

double Vector::dotProduct(Vector x, Vector y)
{
    if(x.getDimension() != y.getDimension()) 
    {
        char opType[] = "Dot Product";
        throw Operation_With_Unmatched_Dimension(x.getDimension() , y.getDimension() , opType);
    }
    else
    {
        double result = 0;
        for(size_t i=0; i<x.getDimension(); i++)
            result += x[i] * y[i];

        return result;
    }
    
}

double Vector::angleBetween(Vector x, Vector y)
{
    double length_x = x.computeLength();
    double length_y = y.computeLength();

    double dot_product = Vector::dotProduct(x , y);

    return acos((dot_product) / (length_x * length_y));
}



