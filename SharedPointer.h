#ifndef SHAREDPOINTER_H
#define SHAREDPOINTER_H

#include "Pointer.h"
#include <cstdlib>
#include "Exception.h"

namespace DTLib
{

template <typename T>
class SharedPointer : public Pointer<T>
{
protected:
    int* m_ref;

    void assign(const SharedPointer<T>& obj)
    {
        this->m_ref = obj.m_ref;
        this->m_pointer = obj.m_pointer;

        if( this->m_ref )  // obj 也可能为空
        {
            (*this->m_ref)++;
        }
    }

public:
    SharedPointer(T* p = NULL) : m_ref(NULL)
    {
        if( p )
        {
            this->m_ref = static_cast<int*>(std::malloc(sizeof(int)));

            if( this->m_ref )
            {
                *(this->m_ref) = 1;
                this->m_pointer = p;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to creat SharedPointer object ...");
            }
        }
    }

    SharedPointer(const SharedPointer<T>& obj) : Pointer<T>(NULL)  // 初始化赋值调用，这里为了消除警告所以显示调用父类构造函数
    {
        assign(obj);
    }

    SharedPointer<T>& operator= (const SharedPointer<T>& obj)
    {
        if( this != &obj) // 避免自賦值
        {
            clear();
            /*
                也許當前的 SharedPointer 對象已經指向另一片堆空間,
                所以赋值前應該先置空，让其不再指向任何的堆空間.
            */
            assign(obj);
        }

        return *this;
    }

    void clear()
    {
        T* toDel = this->m_pointer;
        int* ref = this->m_ref;

        this->m_ref = NULL;
        this->m_pointer = NULL;

        /*
            現在有一個智能指針對象不再指向一片堆空間,
            這片堆空間所關聯著的計數變量應該-1
        */
        if( ref )
        {
            (*ref)--;

            if( *ref == 0 )
            {
                free(ref);

                delete toDel;
            }
        }
    }

    ~SharedPointer()
    {
        clear();
    }
};

/* 通过全局函数的方式来重载相等比較操作符 */
template <typename T>
bool operator== (const SharedPointer<T>& l, const SharedPointer<T>& r)
{
    return (l.get() == r.get());  // 所指向的堆空间是一个就相等
    /*
        get 函數並不是一個 const 成員函數, 所以不能被 const 對象所調用.

        怎麼改呢?
        => 將函數做成 const 成員函數
    */
}

/* 通过全局函数的方式来重载不等比較操作符 */
template <typename T>
bool operator!= (const SharedPointer<T>& l, const SharedPointer<T>& r)
{
    return !(l == r);
    // 不相等比較操作符的重载使用相等比較操作符的重載来实现
}

}

#endif // SHAREDPOINTER_H
