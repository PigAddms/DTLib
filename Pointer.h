#ifndef POINTER_H
#define POINTER_H

#include "Object.h"

namespace DTLib
{

template < typename T >
class Pointer : public Object
{
protected:
    T* m_pointer;

public:
    Pointer(T* p = NULL)
    {
        m_pointer = p;
    }

    T* operator-> ()
    {
        return m_pointer;
    }

    T& operator* ()
    {
        return *m_pointer;
    }
    // ����ɂ���������Ҫ׃�� const �ɆT����, ������Ҫ�M�� const ���d

    const T* operator-> () const
    {
        return m_pointer;
    }

    const T& operator* () const
    {
        return *m_pointer;
    }
    // ����ɂ������ǌ����� const �ɆT���������d����


    bool isNull() const
    {
        return (m_pointer == NULL);
    }

    T* get() const
    {
        return m_pointer;
    }

    /*
        ��Ҫ���F����������?

        ����Ҫ��������, ����̳���플Ӹ�� Object,
        ֻҪ��ʵ�־��������������Pointer ��������һ��������.
    */
};

}

#endif // POINTER_H
