#ifndef DYNAMICLIST_H
#define DYNAMICLIST_H

#include "SeqList.h"
#include "Exception.h"

namespace DTLib
{
template <typename T>
class DynamicList : public SeqList<T>
{
protected:
    int m_capacity;   //顺序存储空间的大小

public:
    DynamicList(int capacity)    //申请顺序存储空间
    {
        this->m_array = new T[capacity];
        if( this->m_array != NULL )
        {
            this->m_length = 0;
            this->m_capacity = capacity;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No menory to create DynamicList object ...");
        }
    }

    int capacity() const
    {
        return m_capacity;
    }

    /* 重置顺序存储空间的大小,觉着当前存储空间太大或太小了，要重置 */
    void resize(int capacity)
    {
        if( m_capacity != capacity )// 只有當想要的順序存儲空間大小, 和當前的順序存儲空間大小不同的時候, 才來做實際的工作.
        {
            T* array = new T[capacity];// 沒有直接操作 this->m_array 是為了重置順序空間大小後, 保证数据结构没有丢失，所以要让 m_array 还是指向原来的堆空间.

            if( array != NULL )
            {
                int length = (this->m_length < capacity ? this->m_length : capacity);

                for( int i=0; i<length; i++ )
                {
                    array[i] = this-> m_array[i];
                    /*
                        这里賦值操作可能发生异常, 当发生异常的时候, 三个成员变量值没有發生任何的改变, 當前的线性表對象依然合法可用的,
                        只不过 array 指向的堆空间将会发生泄漏，但是对于 resize 函数是无法顾全的.

                        因为 array 里面存储的是 T 的泛指类型, 这个泛指类型赋值操作符有可能被重载, 如果重载的實現里面是會發生异常的，
                        这里我们就不能顾全了. 如果在这里发生了异常, 则就是泛指类型 T 导致的，也就是第三方工程師的代碼的問題, 這樣的問題就交由第三方工程師自己來考慮了.
                    */
                }

                T* temp = this->m_array;
                /*
                    這樣寫是為了在最後來 delete 先前的順序存儲空間, 由於 T 是泛指類型,
                    如果T是一個類類型, 並且在類的析構函數裡面拋出了一個異常, 直接 delete[] this -> m_array ,
                    函數就要異常返回了, 下面三條賦值操作是沒有辦法執行到, 就無法保證當前的線性表還是合法可用的,
                    因此這個 resize 函數就並不是異常安全的, 但凡發生異常, 這個線性表對象就不可以用了.
                */
                this->m_array = array;  //语句不会发生异常
                this->m_length = length;    //语句不会发生异常
                this->m_capacity = capacity;    //语句不会发生异常

                delete[] temp;
                /*
                    即便发生了异常，然后异常返回，三个成员变量的值已经合法，则当前线性表对象也是合法可用的,
                    這樣就做到了異常安全.
                */
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No enough to resize DynamicList object ...");
            }
        }
    }

    ~DynamicList()
    {
        delete[] this->m_array;
    }
};

}

#endif // DYNAMICLIST_H
