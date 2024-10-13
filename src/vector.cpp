#include<cmath>
#include <math.h>
#include"vector.h"

template<typename T>
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



