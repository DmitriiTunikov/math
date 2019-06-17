#include <cstring>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <new>          // std::nothrow
#include <QString>
#include <string.h>

#include "Vector.h"
#include "ILog.h"

#define REPORT(MSG) \
    QString qmsg("[VECTOR]:  "); \
    qmsg += QString(MSG); \
    qmsg += "\n\t\tFile: "; \
    qmsg += __FILE__; \
    qmsg += "\n\t\tLine: "; \
    qmsg += QString::number(__LINE__); \
    qmsg += "\n\t\tFunction: "; \
    qmsg += __FUNCTION__; \
    ILog::report(qmsg.toStdString().c_str())

#define ABS(x) ((x) > 0 ? (x) : -(x))

#define CHECK_DIM(name) \
    if ((name) == NULL) \
    {\
        REPORT("Null argument to vector"); \
        return ERR_WRONG_ARG; \
    } \
    if ((name)->getDim() != m_size) \
    { \
        REPORT("Dimension mismatch"); \
        return ERR_DIMENSIONS_MISMATCH; \
    }

#define CHECK_NULL_ERROR_AS_NULL(name) \
    if ((name) == NULL) \
        return NULL;

Vector::Vector(unsigned int size, double *vals) :
    m_size(size), m_vals(vals)
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
    CHECK_DIM(right);
    for (unsigned i = 0; i < m_size; i++)
    {
        double elem;
        right->getCoord(i, elem);
        m_vals[i] += elem;
    }

   return ERR_OK;
}

int Vector::subtract(IVector const* const right)
{
    CHECK_DIM(right);
    for (unsigned i = 0; i < m_size; i++)
    {
        double elem;
        right->getCoord(i, elem);
        m_vals[i] -= elem;
    }

   return ERR_OK;
}

int Vector::multiplyByScalar(double scalar)
{
    for (unsigned i = 0; i < m_size; i++)
        m_vals[i] *= scalar;

    return ERR_OK;
}

int Vector::dotProduct(IVector const* const right, double& res) const
{
    CHECK_DIM(right)

    res = 0;
    for (unsigned i = 0; i < m_size; i++)
    {
        double elem;
        right->getCoord(i, elem);
        res += m_vals[i] + elem;
    }

    return ERR_OK;
}

IVector* Vector::add(IVector const* const left, IVector const* const right)
{
    CHECK_NULL_ERROR_AS_NULL(left)
    CHECK_NULL_ERROR_AS_NULL(right)

    IVector *res = left->clone();
    if (res == NULL)
        return NULL;

    if (res->add(right) != ERR_OK)
    {
        delete res;
        return NULL;
    }

    return res;
}

IVector* Vector::subtract(IVector const* const left, IVector const* const right)
{
    CHECK_NULL_ERROR_AS_NULL(left)
    CHECK_NULL_ERROR_AS_NULL(right)

    IVector *res = left->clone();
    if (res == NULL)
        return NULL;

    if (res->subtract(right) != ERR_OK)
    {
        delete res;
        return NULL;
    }

    return res;
}

IVector* Vector::multiplyByScalar(IVector const* const left, double scalar)
{
    CHECK_NULL_ERROR_AS_NULL(left);

    IVector *res = left->clone();
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
    CHECK_DIM(right);

    double self_norm = 0, right_norm = 0;
    int err_res;

    err_res = norm(type, self_norm);
    if (err_res != ERR_OK)
        return err_res;

    err_res = right->norm(type, right_norm);
    if (err_res != ERR_OK)
        return err_res;

    result = self_norm > right_norm;
    return ERR_OK;
}

int Vector::lt(IVector const* const right, NormType type, bool& result) const
{
    CHECK_DIM(right);

    double self_norm = 0, right_norm = 0;
    int err_res;

    err_res = norm(type, self_norm);
    if (err_res != ERR_OK)
        return err_res;

    err_res = right->norm(type, right_norm);
    if (err_res != ERR_OK)
        return err_res;

    result = self_norm < right_norm;
    return ERR_OK;
}

int Vector::eq(IVector const* const right, NormType type, bool& result, double precision) const
{
    CHECK_DIM(right);

    double self_norm = 0, right_norm = 0;
    int err_res;

    err_res = norm(type, self_norm);
    if (err_res != ERR_OK)
        return err_res;

    err_res = right->norm(type, right_norm);
    if (err_res != ERR_OK)
        return err_res;

    result = self_norm == right_norm;
    return ERR_OK;
}

unsigned int Vector::getDim() const
{
    return m_size;
}

int Vector::norm(NormType type, double& res) const
{
    res = 0;

    switch (type)
    {
    case NORM_1:
        for (unsigned i = 0; i < m_size; i++)
            res += m_vals[i] * m_vals[i];
        res = std::sqrt(res);
        break;
    case NORM_2:
        for (unsigned i = 0; i < m_size; i++)
            res += abs(m_vals[i]);
        break;
    case NORM_INF:
        for (unsigned i = 0; i < m_size; i++)
            res = std::max(res, ABS(m_vals[i]));
        break;
    case DIMENSION_NORM:
        for (unsigned i = 0; i < m_size; i++)
            res += std::pow(ABS(m_vals[i]), m_size);
        res = std::pow(res, 1.0 / m_size);
        break;
    default:
        REPORT("Undefined norm as argument");
        return ERR_NORM_NOT_DEFINED;
        break;
    }
    return ERR_OK;
}

int Vector::setCoord(unsigned int index, double elem)
{
    if (index >= m_size)
    {
        REPORT("Vector index is out of range");
        return ERR_OUT_OF_RANGE;
    }
    m_vals[index] = elem;
    return ERR_OK;
}

int Vector::getCoord(unsigned int index, double & elem) const
{
    if (index >= m_size)
    {
        REPORT("Vector index is out of range");
        return ERR_OUT_OF_RANGE;
    }
    elem = m_vals[index];
    return ERR_OK;
}

int Vector::setAllCoords(unsigned int dim, double* coords)
{
    if (dim != m_size)
    {
        REPORT("Vectors dimensions mismatch");
        return ERR_DIMENSIONS_MISMATCH;
    }
    m_size = dim;
    std::memcpy(m_vals, coords, sizeof(double) * dim);

    return ERR_OK;
}

int Vector::getCoordsPtr(unsigned int & dim, double const*& elem) const
{
    dim = m_size;
    elem = m_vals;

    return ERR_OK;
}

IVector* Vector::clone() const
{
    return Vector::createVector(m_size, m_vals);
}
