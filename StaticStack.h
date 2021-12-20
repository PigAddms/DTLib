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
        // ��ʾ׃����ʾλ�Þ�-1, ��ǰ���e��һ��Ԫ�ض��]��, �]�����^�ė��.
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
                �ȸ�ֵ����֤�쳣��ȫ.

                ��������xֵ�����ĕr��, �п��ܮa������, e �@������Ԫ���Ƿ�ָ���,
                Ҳ�Ǿ��f e ����Ϳ����ǂ�����, �fһ�@�����͵��xֵ��������
                �xֵ�ĕr��a���ˮ���, �҂�Ҫ���C������ȫ, �����@�e�l���ˮ���,
                ���Ȳ��Ġ�B�]�аl����׃.
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
        if( m_size > 0 ) // ��ǰ�ė����Д���Ԫ�ز��ܳ���
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
