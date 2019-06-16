#ifndef SET_H
#define SET_H
#include "ISet.h"
#include "Vector.h"

class SHARED_EXPORT Set : ISet
{
public:
     int getId() const
     {
         return INTERFACE_0;
     }

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
        Iterator(const Iterator& other) = delete;
        void operator=(const Iterator& other) = delete;
        unsigned int m_idx;
        Set* m_set;
    };

    IIterator* end();
    IIterator* begin();

    int deleteIterator(IIterator * pIter);
    int getByIterator(IIterator const* pIter, IVector*& pItem) const;

    /*dtor*/
    ~Set();
private:
    Vector* myCreateVector(unsigned int size, double const* vals);
    int copyAndDeleteData();
    void cleanMemory(IVector const ** data, unsigned int size);
    Set(IVector const ** data);
    // DATA
    IVector const ** m_data;
    unsigned int m_curIdx;
    unsigned int m_dim;
};

#endif // SET_H
