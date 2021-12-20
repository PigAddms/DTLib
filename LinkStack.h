#ifndef LINKSTACK_H
#define LINKSTACK_H

#include "Stack.h"
#include "LinkList.h"


namespace DTLib
{

template < typename T >
class LinkStack : public Stack<T>
{
protected:
    LinkList<T> m_list;
    // 組合使用單鏈表類
public:
    // 不用構造函數

    void push(const T& e)   // O(1)
    {
        m_list.insert(0, e);
        // 頭插法, 將新的數據元素插入到成員鏈表的頭部.
    }

    void pop()  // O(1)
    {
        if( m_list.length() > 0 )
        {
            m_list.remove(0);
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No element in current stack...");
        }
    }

    T top() const   // O(1)
    {
        if( m_list.length() > 0 )
        {
            return m_list.get(0);
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No element in current stack...");
        }
    }

    void clear()    // O(n)
    {
        m_list.clear();
    }

    int size() const    // O(1)
    {
        return m_list.length();
        /*
            所有的棧中的數據元素是存在成員鏈表裡面的,
            成員鏈表的長度就是當前棧裡面數據元素的個數.
        */
    }

};

}

#endif // LINKSTACK_H
