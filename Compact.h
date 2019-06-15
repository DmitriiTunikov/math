#ifndef COMPACT_H
#define COMPACT_H

#include "ICompact.h"
#include "IVector.h"

class SHARED_EXPORT Compact : ICompact
{
public:
    int getId() const
    {
        return INTERFACE_0;
    }

    /*factories*/
    static Compact* createCompact(IVector const* const begin, IVector const* const end, IVector const* const step = 0);

    int deleteIterator(ICompact::IIterator * pIter);
    int getByIterator(ICompact::IIterator const* pIter, IVector*& pItem) const;

    ICompact::IIterator* end(IVector const* const step = 0);
    ICompact::IIterator* begin(IVector const* const step = 0);

    int isContains(IVector const* const vec, bool& result) const;
    int isSubSet(ICompact const* const other) const;
    int isIntersects(ICompact const* const other, bool& result) const;
    int getNearestNeighbor(IVector const* vec, IVector *& nn) const;

    Compact* clone() const;

    ~Compact();

    class Iterator : ICompact::IIterator
    {
    public:
        //adds step to current value in iterator
        int doStep();

        //change step
        int setStep(IVector const* const step);

    protected:
        Iterator(Compact const* const compact, int pos, IVector const* const step);
        /*dtor*/
        ~Iterator();
    };

private:
    Compact(IVector const* const begin, IVector const* const end, IVector const* const step = 0);

    IVector const* m_begin;
    IVector const* m_end;
    IVector const* m_step;
};

#endif // COMPACT_H

