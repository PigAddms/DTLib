#ifndef SEQLIST_H
#define SEQLIST_H

#include "List.h"
#include "Exception.h"

namespace DTLib
{

template <typename T>
class SeqList : public List<T>
{
protected:
    T* m_array;       //顺序存储空间，子类实现，这里不赋值指针
    int m_length;     //当前线性表长度

public:
    bool insert(int i, const T& e)      // n + 5 => O(n+5) => O(n)      O(5) => O(1)
    {
        bool ret = ((0 <= i) && (i <= m_length));   // 1
        ret = ret && (m_length < capacity());       // 1
        //當前線性表長度應該小於順序存儲空間的最大容量

        if( ret )
        {
            // 從最後一個位置的數據元素開始, 逐個的向後挪動一個位置, 挪動到目標位置為止.
            for(int p=m_length-1; p>=i; p--)
            {
                m_array[p + 1] = m_array[p];    // n, 0
                // 这里数组操作符实质是指针运算
            }

            m_array[i] = e;             // 1
            // 將新元素插入
            m_length++;                 // 1
            // 線性表長度+1
        }

        return ret;                     // 1
    }

    bool insert(const T& e)
    {
        return insert(m_length, e);
    }

    bool remove(int i)   // O(n)
    {
        bool ret = ((0 <= i) && (i < m_length));

        if( ret )
        {
            // 將 i 之後的每一個數據元素, 向前挪動一個位置.
            for(int p = i; p<m_length-1; p++)       // n-1
            {
                m_array[p] = m_array[p + 1];
            }

            m_length--;
        }

        return ret;
    }

    bool set(int i, const T& e)
    {
        bool ret = ((0 <= i) && (i < m_length));

        if( ret )// 只有目標位置合法, 才可以設置值
        {
            m_array[i] = e;
        }

        return ret;
    }

/* 通过参数 e 来返回目标位置，而没有通过返回值返回，是由于目标位置可能不合法，不合法就返回 false，也就是 get() 操作用来说明当前操作是否合法*/
    bool get(int i, T& e) const
    {
        bool ret = ((0 <= i) && (i < m_length));

        if( ret ) // 只有目標位置合法, 才能拿到想要的數據元素的值
        {
            e = m_array[i];
        }

        return ret;
    }

    int find(const T& e) const     // O(n)
    {
        int ret = -1;

        for(int i=0; i<m_length; i++)
        {
            if( m_array[i] == e )
            {
                ret = i;
                break;
            }
        }

        return ret;
    }

    int length() const
    {
        return m_length;
    }

    void clear()
    {
        m_length = 0; // 清空一個線性表意味著, 這個線性表裡面沒有任何的數據元素了.
    }

    // 顺序存储线性表的数组访问方式
    T& operator[] (int i)
    {
        bool ret = ((0 <= i) && (i < m_length));
        /*
            在做數組訪問的時候, 要先確認當前線性表裡面, 對應的目標位置有沒有元素,
            如果沒有元素, 拋一個異常說明越界了.
        */

        if( ret )
        {
            return m_array[i];
        }
        else
        {
            THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ..." );
        }
    }

    // 这里考虑了 const 和非 const 对象
    T operator[] (int i) const
    {
        return (const_cast<SeqList<T>&>(*this))[i];
    }

    //顺序存储空间的容量
    virtual int capacity() const = 0; // 顺序存储空间的指定没有在这个类中指定，所以具体实现要放在子类中完成.
};

}
#endif // SEQLIST_H
