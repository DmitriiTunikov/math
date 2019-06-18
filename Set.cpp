#include "Set.h"
#include "error.h"
#include <stdio.h>
#include <new>          // std::nothrow

#include "ILog.h"
#include <cstring>
#include <QScopedPointer>
#include <QString>

#define REPORT(MSG) \
    QString qmsg("[SET_DT20]:  "); \
    qmsg += QString(MSG); \
    qmsg += "\n\t\tFile: "; \
    qmsg += __FILE__; \
    qmsg += "\n\t\tLine: "; \
    qmsg += QString::number(__LINE__); \
    qmsg += "\n\t\tFunction: "; \
    qmsg += __FUNCTION__; \
    ILog::report(qmsg.toStdString().c_str())

ISet::IIterator::IIterator(ISet const* const set, int pos)
{
}

IVector* Set::myCreateVector(unsigned int size, double const* vals){
    return Vector::createVector(size, vals);
}

int Set::createDoubleVec(IVector** vec, size_t idx)
{
    QScopedPointer<double, QScopedPointerArrayDeleter<double> > tmpDouble(new double[m_dim]);
    if (tmpDouble.isNull())
    {
        cleanMemory(vec, idx);
        REPORT("can't allocate memory");
        return ERR_MEMORY_ALLOCATION;
    }
    vec[idx] = myCreateVector(m_dim, tmpDouble.data());
        return ERR_OK;
}

Set::Set(IVector ** data, unsigned int dim) :
    m_data(data), m_curSize(0), m_dim(dim)
{
}

int Set::copyAndDeleteData(IVector ** dst, IVector ** src, unsigned int size)
{
    if (src == NULL || dst == NULL || size < 0)
    {
        REPORT("can't allocate mem for tmp vector* arr");
        return ERR_MEMORY_ALLOCATION;
    }

    memcpy(dst, src, size);
    for (size_t i = 0; i < size; i++)
    {
        int ec = createDoubleVec(dst, i);
        if (ec != ERR_OK)
            return ec;
        memcpy(dst[i], src[i], m_dim);
    }
    cleanMemory(const_cast<IVector**>(src), size);
    return ERR_OK;
}

void Set::cleanMemory(IVector ** data, unsigned int size)
{
    for (size_t i = 0; i < size; i++)
        delete data[i];
    delete[] data;
}

int Set::put(IVector const * const item){
    if (item == NULL){
        REPORT("error while put item to set - item == NULL");
        return ERR_WRONG_ARG;
    }

    IVector ** tmpData = new (std::nothrow) IVector *[m_curSize + 1];
    if (tmpData == NULL)
    {
        REPORT("can't allocate memory");
        return ERR_MEMORY_ALLOCATION;
    }
    if (m_curSize == 0)
    {
        m_data = new (std::nothrow) IVector *[m_curSize];
        if (m_data == NULL)
        {
            REPORT("can't allocate memory");
            return ERR_MEMORY_ALLOCATION;
        }
    }
    int ec = copyAndDeleteData(tmpData, m_data, m_curSize);
    if (ec != ERR_OK)
    {
        cleanMemory(tmpData, m_curSize);
        return ec;
    }
    m_data = tmpData;
    m_data[m_curSize++] = item->clone();
    return ERR_OK;
}

int Set::get(unsigned int index, IVector*& pItem) const{
    if (index < 0 || index > m_curSize - 1){
        REPORT("error while getting set item by index - wrong index");
        return ERR_OUT_OF_RANGE;
    }
    pItem = m_data[index]->clone();

    return ERR_OK;
}

int Set::remove(unsigned int index)
{
    IVector ** tmpData = new (std::nothrow) IVector*[m_curSize - 1];
    if (tmpData == NULL)
    {
        REPORT("can't allocate memory");
        return ERR_MEMORY_ALLOCATION;
    }
    unsigned int i = 0;
    for (i = 0; i < index; i++)
    {
        int ec = createDoubleVec(tmpData, i);
        if (ec != ERR_OK)
            return ec;
        memcpy(tmpData[i], m_data, m_dim);
    }
    i++;
    for ( ; i < m_curSize; i++)
    {
        int ec = createDoubleVec(tmpData, i);
        if (ec != ERR_OK)
            return ec;
        memcpy(tmpData[i - 1], m_data, m_dim);
    }
    cleanMemory(m_data, m_curSize);
    m_data = tmpData;
    m_curSize--;
    return ERR_OK;
}

int Set::contains(IVector const * const pItem, bool & rc) const{
    if (pItem == NULL){
        REPORT("error while contains set method - wrong item");
        return ERR_WRONG_ARG;
    }
    rc = false;
    for (unsigned int i = 0; i < m_curSize; i++){
        int ec = m_data[i]->eq(pItem, IVector::NormType::NORM_1, rc, 0.001);
        if (ec != ERR_OK)
            return ec;
        if (rc == true)
            return ERR_OK;
    }

    return ERR_OK;
}

unsigned int Set::getSize() const{
    return m_curSize;
}


ISet* Set::createSet(unsigned int R_dim){
    Set *res = new (std::nothrow) Set(NULL, R_dim);
    if (res == NULL)
    {
        REPORT("Set::createSet - can't allocate memory for Set");
        return NULL;
    }

    return res;
}

int Set::clear(){
    for (unsigned int i = 0; i < m_curSize; i++)
        delete m_data[i];
    delete[] m_data;
    m_curSize = 0;

    return ERR_OK;
}

int Set::deleteIterator(IIterator * pIter){
    delete reinterpret_cast<Iterator*>(pIter);
    return ERR_OK;
}

Set::Iterator::Iterator(Set const* const set, int pos) : IIterator(set, pos), m_set(set){
    m_idx = pos;
}

int Set::getByIterator(const IIterator *pIter, IVector *&pItem) const{
    if (pIter == NULL)
        return ERR_WRONG_ARG;

    const Iterator * it = reinterpret_cast<const Iterator *>(pIter);
    if (it == NULL)
    {
        REPORT(" Set::getByIterator - can't cast iter");
        return ERR_WRONG_ARG;
    }

    pItem = m_data[it->m_idx]->clone();
    return ERR_OK;
}

int Set::Iterator::next(){
    if (isEnd())
        return ERR_OUT_OF_RANGE;

    ++m_idx;
    return ERR_OK;
}

ISet::IIterator* Set::end(){
    return new Iterator(this, m_curSize - 1);
}

ISet::IIterator* Set::begin(){
    return new Iterator(this, 0);
}

Set::~Set(){
    clear();
}

int Set::Iterator::prev(){
    if (isBegin())
        return ERR_OUT_OF_RANGE;

    --m_idx;
    return ERR_OK;
}

bool Set::Iterator::isEnd() const{
    return m_idx == m_set->m_curSize - 1;
}

bool Set::Iterator::isBegin() const{
    return m_idx == 0;
}
