#ifndef SET_H
#define SET_H
#include "ISet.h"
#include "Vector/Vector.h"

class SHARED_EXPORT Set : ISet
{
public:
     int getId() const;

    /*factories*/
    static ISet* createSet(unsigned int R_dim);

    int put(IVector const* const item);
    int get(unsigned int index, IVector*& pItem) const;
    int remove(unsigned int index);
    int contains(IVector const* const pItem, bool & rc) const;
    unsigned int getSize() const;
    int clear() ;

    class Iterator : ISet::IIterator
    {
    public:
        friend class Set;
        int next();
        int prev();
        bool isEnd() const;
        bool isBegin() const;
        Iterator(ISet const* const set, int pos);
    private:
        Iterator() = delete;
        IIterator(const IIterator& other) = delete;
        void operator=(const IIterator& other) = delete;
        unsigned int m_curIdx;
        unsigned int m_setSize;
    };

    IIterator* end();
    IIterator* begin();

    int deleteIterator(IIterator * pIter);
    int getByIterator(IIterator const* pIter, IVector*& pItem) const;

    /*dtor*/
    ~Set();
private:
    Vector const ** m_data;
    unsigned int m_size;
    unsigned int m_curIdx;
};

#endif // SET_H
