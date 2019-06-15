#include "Vector.h"

#include <cstring>

IVector* Vector::createVector(unsigned int size, double const* vals)
{

    std::memcpy(vals, _vals, sizeof(double) * _size);
}
