//
// Created by dimat on 15.06.2019.
//

#include "Set.h"
#include "error.h"
#include <stdio.h>
#include <new>          // std::nothrow

//#include "Logger/Logger.h"

ISet::IIterator::IIterator(ISet const* const set, int pos)
{
}

Set::Set(Vector const ** data, unsigned int size) :
    m_data(data), m_size(size), m_curIdx(0)
{
}

int Set::put(IVector const* const item){
    if (item == NULL){
        //Logger::print(QString("error while put item to set - item == NULL"));
        return ErrorEnum::ERR_WRONG_ARG;
    }
    const Vector * const tmp = dynamic_cast<const Vector *const >(item);
    if (tmp == NULL)
    {
        //Logger::print(QString("error while making cast from IVector to Vector"));
        return ErrorEnum::ERR_WRONG_ARG;
    }
    m_data[m_curIdx++] = tmp;

    return ERR_OK;
}

int Set::get(unsigned int index, IVector*& pItem) const{
    if (index < 0 || index > m_size - 1){
        //Logger::print(QString("error while getting set item by index - wrong index"));
        return ErrorEnum::ERR_OUT_OF_RANGE;
    }
    pItem = m_data[index]->clone();

    return ERR_OK;
}

int Set::remove(unsigned int index){
    for (unsigned int i = index; i < m_size - 1; i++)
        m_data[i] = m_data[i + 1];
    m_curIdx--;

    return ERR_OK;
}

int Set::contains(IVector const* const pItem, bool & rc) const{
    if (pItem == NULL){
        //Logger::print(QString("error while contains set method - wrong item"));
        return ErrorEnum::ERR_WRONG_ARG;
    }
    rc = true;
    for (unsigned int i = 0; i < m_size; i++){
        m_data[i]->eq(pItem, IVector::NormType::NORM_1, rc, 0.001);
        if (rc == false)
            return ErrorEnum::ERR_OK;
    }

    return ERR_OK;
}

unsigned int Set::getSize() const{
    return m_size;
}


ISet* Set::createSet(unsigned int R_dim){
    if (R_dim == 0)
        return NULL;

    Vector const **data = new (std::nothrow) Vector const*[R_dim];
    if (data == NULL)
        return NULL;

    Set *res = new (std::nothrow) Set(data, R_dim);
    if (res == NULL)
        delete [] data;

    return res;
}

int Set::clear(){
    for (unsigned int i = 0; i < m_size; i++)
        delete m_data[i];
    delete[] m_data;
    m_size = 0;
    m_curIdx = 0;

    return ERR_OK;
}


int Set::deleteIterator(IIterator * pIter){
    delete pIter;
    return ErrorEnum::ERR_OK;
}

Set::Iterator::Iterator(ISet const* const set, int pos) : IIterator(set, pos){
    m_setSize = set->getSize();
    m_curIdx = pos;
}

int Set::getByIterator(const IIterator *pIter, IVector *&pItem) const{
    if (pIter == NULL)
        return ErrorEnum::ERR_WRONG_ARG;

    const Iterator * it = dynamic_cast<const Iterator *>(pIter);
    if (it == NULL)
        return ErrorEnum::ERR_WRONG_ARG;

    pItem = m_data[it->m_curIdx]->clone();
    return ErrorEnum::ERR_OK;
}

int Set::Iterator::next(){
    if (m_curIdx == m_setSize)
        return ErrorEnum::ERR_OUT_OF_RANGE;

    ++m_curIdx;
    return ErrorEnum::ERR_OK;
}

ISet::IIterator* Set::end(){
    return new Iterator(this, m_size - 1);
}

ISet::IIterator* Set::begin(){
    return new Iterator(this, 0);
}

Set::~Set(){
    clear();
}

int Set::Iterator::prev(){
    if (m_curIdx == 0)
        return ErrorEnum::ERR_OUT_OF_RANGE;

    --m_curIdx;
    return ErrorEnum::ERR_OK;
}

bool Set::Iterator::isEnd() const{
    return m_curIdx == m_setSize - 1;
}

bool Set::Iterator::isBegin() const{
    return m_curIdx == 0;
}
