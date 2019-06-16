#include "Compact.h"
#include "ILog.h"

ICompact::IIterator::IIterator(const ICompact *const compact, int pos, const IVector *const step)
{
}

Compact::Compact(IVector const* const begin, IVector const* const end, IVector const* const step) :
    m_begin(begin), m_end(end), m_step(step)
{
}

/*factories*/
Compact* Compact::createCompact(IVector const* const begin, IVector const* const end, IVector const* const step)
{
}

int Compact::deleteIterator(ICompact::IIterator * pIter)
{
}

int Compact::getByIterator(ICompact::IIterator const* pIter, IVector*& pItem) const
{
}

ICompact::IIterator* Compact::end(IVector const* const step)
{
}

ICompact::IIterator* Compact::begin(IVector const* const step)
{
}

int Compact::isContains(IVector const* const vec, bool& result) const
{
}

int Compact::isSubSet(ICompact const* const other) const
{
}

int Compact::isIntersects(ICompact const* const other, bool& result) const
{
}

int Compact::getNearestNeighbor(IVector const* vec, IVector *& nn) const
{
}

Compact* Compact::clone() const
{
}

Compact::~Compact()
{
}


int Compact::Iterator::doStep()
{
}

int Compact::Iterator::setStep(IVector const* const step)
{
}


Compact::Iterator::~Iterator()
{
}

Compact::Iterator::Iterator(Compact const* const compact, int pos, IVector const* const step) :
    IIterator(compact, pos, step)
{
}

