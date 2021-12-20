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
    T* m_array;       //˳��洢�ռ䣬����ʵ�֣����ﲻ��ֵָ��
    int m_length;     //��ǰ���Ա���

public:
    bool insert(int i, const T& e)      // n + 5 => O(n+5) => O(n)      O(5) => O(1)
    {
        bool ret = ((0 <= i) && (i <= m_length));   // 1
        ret = ret && (m_length < capacity());       // 1
        //��ǰ���Ա��L�ȑ�ԓС����惦���g���������

        if( ret )
        {
            // ������һ��λ�õĔ���Ԫ���_ʼ, ����������Ų��һ��λ��, Ų�ӵ�Ŀ��λ�Þ�ֹ.
            for(int p=m_length-1; p>=i; p--)
            {
                m_array[p + 1] = m_array[p];    // n, 0
                // �������������ʵ����ָ������
            }

            m_array[i] = e;             // 1
            // ����Ԫ�ز���
            m_length++;                 // 1
            // ���Ա��L��+1
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
            // �� i ֮���ÿһ������Ԫ��, ��ǰŲ��һ��λ��.
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

        if( ret )// ֻ��Ŀ��λ�úϷ�, �ſ����O��ֵ
        {
            m_array[i] = e;
        }

        return ret;
    }

/* ͨ������ e ������Ŀ��λ�ã���û��ͨ������ֵ���أ�������Ŀ��λ�ÿ��ܲ��Ϸ������Ϸ��ͷ��� false��Ҳ���� get() ��������˵����ǰ�����Ƿ�Ϸ�*/
    bool get(int i, T& e) const
    {
        bool ret = ((0 <= i) && (i < m_length));

        if( ret ) // ֻ��Ŀ��λ�úϷ�, �����õ���Ҫ�Ĕ���Ԫ�ص�ֵ
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
        m_length = 0; // ���һ�����Ա���ζ��, �@�����Ա��e��]���κεĔ���Ԫ����.
    }

    // ˳��洢���Ա��������ʷ�ʽ
    T& operator[] (int i)
    {
        bool ret = ((0 <= i) && (i < m_length));
        /*
            �������M�L���ĕr��, Ҫ�ȴ_�J��ǰ���Ա��e��, ������Ŀ��λ���Л]��Ԫ��,
            ����]��Ԫ��, ��һ�������f��Խ����.
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

    // ���￼���� const �ͷ� const ����
    T operator[] (int i) const
    {
        return (const_cast<SeqList<T>&>(*this))[i];
    }

    //˳��洢�ռ������
    virtual int capacity() const = 0; // ˳��洢�ռ��ָ��û�����������ָ�������Ծ���ʵ��Ҫ�������������.
};

}
#endif // SEQLIST_H
