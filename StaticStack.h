#ifndef STATICSTACK_H
#define STATICSTACK_H

#include "Stack.h"
#include "Exception.h"

namespace DTLib
{

template < typename T, int N >
class StaticStack : public Stack<T>
{
protected:
    T m_space[N];
    int m_top;
    int m_size;
public:
    StaticStack()   // O(1)
    {
        m_top = -1;
        // 標示變量表示位置為-1, 當前棧裡面一個元素都沒有, 沒有所謂的棧頂.
        m_size = 0;
    }

    int capacity() const    // O(1)
    {
        return N;
    }

    void push(const T& e)   // O(1)
    {
        if( m_size < N )
        {
            m_space[m_top + 1] = e;
            /*
                先赋值，保证异常安全.

                由於在做賦值操作的時候, 有可能產生異常, e 這個數據元素是泛指類型,
                也是就說 e 的類型可以是個類類型, 萬一這個類類型的賦值操作符在
                賦值的時候產生了異常, 我們要保證異常安全, 即便這裡發生了異常,
                棧內部的狀態沒有發生改變.
            */
            m_top++;
            m_size++;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No space in current stack ...");
        }
    }

    void pop()  // O(1)
    {
        if( m_size > 0 ) // 當前的棧中有數據元素才能出棧
        {
            m_top--;
            m_size--;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No element in current stack ...");
        }
    }

    T top() const   // O(1)
    {
        if( m_size > 0 )
        {
            return m_space[m_top];
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No element in current stack ...");
        }
    }

    void clear()    // O(1)
    {
        m_top = -1;
        m_size = 0;
    }

    int size() const    // O(1)
    {
        return m_size;
    }
};

}

#endif // STATICSTACK_H
