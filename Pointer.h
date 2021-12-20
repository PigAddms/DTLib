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
    // 上面兩個函數不需要變成 const 成員函數, 但是需要進行 const 重載

    const T* operator-> () const
    {
        return m_pointer;
    }

    const T& operator* () const
    {
        return *m_pointer;
    }
    // 上面兩個函數是對應非 const 成員函數的重載函數


    bool isNull() const
    {
        return (m_pointer == NULL);
    }

    T* get() const
    {
        return m_pointer;
    }

    /*
        需要實現析構函數嗎?

        不需要析构函数, 这里继承自頂層父類 Object,
        只要不实现具体的析构函数，Pointer 这个类就是一個抽象类.
    */
};

}

#endif // POINTER_H
