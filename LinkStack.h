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
    // �M��ʹ�Æ�朱��
public:
    // ���Ø��캯��

    void push(const T& e)   // O(1)
    {
        m_list.insert(0, e);
        // �^�巨, ���µĔ���Ԫ�ز��뵽�ɆT朱���^��.
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
            ���еė��еĔ���Ԫ���Ǵ��ڳɆT朱��e���,
            �ɆT朱���L�Ⱦ��Ǯ�ǰ���e�攵��Ԫ�صĂ���.
        */
    }

};

}

#endif // LINKSTACK_H
