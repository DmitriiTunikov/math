#include <cstring>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <new>          // std::nothrow

#include "Vector.h"

#define ABS(x) ((x) > 0 ? (x) : -(x))

#define TO_CONST_VECTOR(name) \
    const Vector* name##_v = dynamic_cast<const Vector *> (name); \
    if (name##_v == NULL) \
        return ERR_WRONG_ARG; \
    if (name##_v->_size != _size) \
        return ERR_DIMENSIONS_MISMATCH;

#define TO_CONST_VECTOR_ERROR_AS_NULL(name) \
    const Vector* name##_v = dynamic_cast<const Vector *> (name); \
    if (name##_v == NULL) \
        return NULL;

Vector::Vector(unsigned int size, double *vals) :
    _size(size), _vals(vals)
{
}

IVector* Vector::createVector(unsigned int size, double const* vals)
{
    if (size == 0 or vals == NULL)
        return NULL;
    double *vals_copy = new (std::nothrow) double[size];
    if (vals_copy == NULL)
        return NULL;
    std::memcpy(vals_copy, vals, sizeof(double) * size);
    Vector* res = new (std::nothrow) Vector(size, vals_copy);
    if (res == NULL)
        delete [] vals_copy;
    return res;
}

int Vector::add(IVector const* const right)
{
    TO_CONST_VECTOR(right)
    for (unsigned i = 0; i < _size; i++)
        _vals[i] += right_v->_vals[i];

    return ERR_OK;
}

int Vector::subtract(IVector const* const right)
{
   TO_CONST_VECTOR(right)
   for (unsigned i = 0; i < _size; i++)
       _vals[i] -= right_v->_vals[i];

   return ERR_OK;
}

int Vector::multiplyByScalar(double scalar)
{
    for (unsigned i = 0; i < _size; i++)
        _vals[i] *= scalar;

    return ERR_OK;
}

int Vector::dotProduct(IVector const* const right, double& res) const
{
    TO_CONST_VECTOR(right)

    res = 0;
    for (unsigned i = 0; i < _size; i++)
        res += _vals[i] + right_v->_vals[i];

    return ERR_OK;
}

IVector* Vector::add(IVector const* const left, IVector const* const right)
{
    TO_CONST_VECTOR_ERROR_AS_NULL(left)
    TO_CONST_VECTOR_ERROR_AS_NULL(right)

    Vector *res = (Vector *)left_v->clone();
    if (res == NULL)
        return NULL;

    if (res->add(right_v) != ERR_OK)
    {
        delete res;
        return NULL;
    }

    return res;
}

IVector* Vector::subtract(IVector const* const left, IVector const* const right)
{
    TO_CONST_VECTOR_ERROR_AS_NULL(left)
    TO_CONST_VECTOR_ERROR_AS_NULL(right)

    Vector *res = (Vector *)left_v->clone();
    if (res == NULL)
        return NULL;

    if (res->subtract(right_v) != ERR_OK)
    {
        delete res;
        return NULL;
    }

    return res;
}

IVector* Vector::multiplyByScalar(IVector const* const left, double scalar)
{
    TO_CONST_VECTOR_ERROR_AS_NULL(left)

    Vector *res = (Vector *)left_v->clone();
    if (res == NULL)
        return NULL;

    if (res->multiplyByScalar(scalar) != ERR_OK)
    {
        delete res;
        return NULL;
    }

    return res;
}

int Vector::gt(IVector const* const right, NormType type, bool& result) const
{
    TO_CONST_VECTOR(right)
    double self_norm = 0, right_norm = 0;
    int err_res;

    err_res = norm(type, self_norm);
    if (err_res != ERR_OK)
        return err_res;

    err_res = right_v->norm(type, right_norm);
    if (err_res != ERR_OK)
        return err_res;

    result = self_norm > right_norm;
    return ERR_OK;
}

int Vector::lt(IVector const* const right, NormType type, bool& result) const
{
    TO_CONST_VECTOR(right)
    double self_norm = 0, right_norm = 0;
    int err_res;

    err_res = norm(type, self_norm);
    if (err_res != ERR_OK)
        return err_res;

    err_res = right_v->norm(type, right_norm);
    if (err_res != ERR_OK)
        return err_res;

    result = self_norm < right_norm;
    return ERR_OK;
}

int Vector::eq(IVector const* const right, NormType type, bool& result, double precision) const
{
    TO_CONST_VECTOR(right)
    double self_norm = 0, right_norm = 0;
    int err_res;

    err_res = norm(type, self_norm);
    if (err_res != ERR_OK)
        return err_res;

    err_res = right_v->norm(type, right_norm);
    if (err_res != ERR_OK)
        return err_res;

    result = self_norm == right_norm;
    return ERR_OK;
}

unsigned int Vector::getDim() const
{
    return _size;
}

int Vector::norm(NormType type, double& res) const
{
    res = 0;

    switch (type)
    {
    case NORM_1:
        for (unsigned i = 0; i < _size; i++)
            res += _vals[i] * _vals[i];
        res = std::sqrt(res);
        break;
    case NORM_2:
        for (unsigned i = 0; i < _size; i++)
            res += abs(_vals[i]);
        break;
    case NORM_INF:
        for (unsigned i = 0; i < _size; i++)
            res = std::max(res, ABS(_vals[i]));
        break;
    case DIMENSION_NORM:
        for (unsigned i = 0; i < _size; i++)
            res += std::pow(ABS(_vals[i]), _size);
        res = std::pow(res, 1.0 / _size);
        break;
    default:
        return ERR_NORM_NOT_DEFINED;
        break;
    }
    return ERR_OK;
}

int Vector::setCoord(unsigned int index, double elem)
{
    if (index >= _size)
        return ERR_OUT_OF_RANGE;
    _vals[index] = elem;
    return ERR_OK;
}

int Vector::getCoord(unsigned int index, double & elem)
{
    if (index >= _size)
        return ERR_OUT_OF_RANGE;
    elem = _vals[index];
    return ERR_OK;
}

int Vector::setAllCoords(unsigned int dim, double* coords)
{
    if (dim != _size)
        return ERR_DIMENSIONS_MISMATCH;
    _size = dim;
    std::memcpy(_vals, coords, sizeof(double) * dim);

    return ERR_OK;
}

int Vector::getCoordsPtr(unsigned int & dim, double const*& elem) const
{
    dim = _size;
    elem = _vals;

    return ERR_OK;
}

IVector* Vector::clone() const
{
    return Vector::createVector(_size, _vals);
}
