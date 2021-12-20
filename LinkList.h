#ifndef LINKLIST_H
#define LINKLIST_H

#include "List.h"
#include "Exception.h"

namespace DTLib
{

template < typename T >
class LinkList : public List<T>
{
protected:
    struct Node : public Object
    {
        T value;
        Node* next;
    };

    // ���˱��_��ָ��� T ���캯�����{��
    mutable struct : public Object // ����]���^���� Object �Ϳ��܌��ȴ沼�ֵĲ�ͬ
    {
        char reserved[sizeof(T)]; // ���M�H�H���ˁ׿��g
        Node* next;
    }m_header;
    /*
        �����x��朱팦��ĕr��, ��Ȼ�����ɆT���M�И���,
        �@���r�����^�Y�c����ĕr��, ���F�����ѽ��������{���κεĘ��캯��.

        �@��������Ԓ, �^�Y�c�����ڃȴ沼������, ��֮ǰ�Ǜ]���κβ��.
        ����H�H�ǲ��ܷ�ָ��� T ��ʲ�N�������{�Ø��캯��.
    */

    int m_length;
    int m_step;
    Node* m_current;

    Node* position(int i) const     //O(n)   n �鷺ָ�ı�ʾ��, ��˼�䌍���� i.
    {
        Node* ret = reinterpret_cast<Node*>(&m_header);

        for(int p = 0; p < i; p++)
        {
            ret = ret->next;
        }

        return ret;
    }

    virtual Node* create()
    {
        return new Node();
    }

    virtual void destroy(Node* pn)
    {
        delete pn;
    }

public:
    LinkList()
    {
        m_header.next = NULL;
        m_length = 0;
        m_step = 1;
        m_current = NULL;
    }

    bool insert(int i, const T& e)      //O(n)
    {
        bool ret = ((0 <= i) && (i <= m_length));

        if( ret )
        {
            Node* node = create();

            if( node != NULL )
            {
                Node* current = position(i);

                node->value = e;
                node->next = current->next;
                current->next = node;

                m_length++;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memery to insert new element...");
            }
        }

        return ret;
    }

    bool insert(const T& e)     //O(n)
    {
        return insert(m_length, e);
    }

    bool remove(int i)      // O(n)
    {
        bool ret = ((0 <= i) && (i < m_length));

        if( ret )
        {
            Node* current = position(i);
            Node* toDel = current->next;

            if( m_current == toDel) // m_current �ǲ����ѽ�ָ�򼴌����h���Ĕ���Ԫ��
            {
                m_current = toDel->next;
                // �Ƅ� m_current, ʹ��ָ�򼴌����h���Ĕ���Ԫ�ص���һ��Ԫ��
            }

            current->next = toDel->next;

            m_length--;

            destroy(toDel);
        }

        return ret;
    }

    bool set(int i, const T& e)     //O(n)
    {
        bool ret = ((0 <= i) && (i < m_length));

        if( ret )
        {
            position(i)->next->value = e;
        }

        return ret;
    }

    bool get(int i, T& e) const     //O(n)
    {
        bool ret = ((0 <= i) && (i < m_length));

        if( ret )
        {
            /*
                ��� get ������ const �ɆT����, �҂����@���ɆT�����e��
                �����޸��κγɆT׃����ֵ.

                ���Ԯ�����Ҫȡ�^�Y�c m_header �@���ɆT׃���ĵ�ַ, ��ʱ����������Ϊ
                �����п���Ҫ�޸ĳɆT׃����ֵ, ���Ծͱ��뱨��.

                ��������� m_header ���ָᘼ��� mutable ����.
            */

            e = position(i)->next->value;
        }

        return ret;
    }

    virtual T get(int i) const      //O(n)
    {
        T ret;
        if( get(i, ret) )
        {
            return ret;
        }
        else
        {
            THROW_EXCEPTION(IndexOutOfBoundsException, "Invalid parameter i to get element ...");
        }
        return ret;
    }

    int find(const T& e) const      //O(n)
    {
        int ret = -1;

        int i = 0;
        Node* node = m_header.next;

        while( node ) // ��朱��M�б�v, �÷�ָ�ķ����J��朱��� n ������Ԫ�� ==> O(n)
        {
            if( node->value == e)
            {
                ret = i;
                break;
            }
            else
            {
                node = node->next;
                i++;
            }
        }

        return ret;
    }

    int length() const      // O(1)
    {
        return m_length;
    }

    void clear()        // O(n)
    {
        while( m_header.next ) // ��v朱��e���ÿһ������Ԫ�� ==> O(n)
        {
            Node* toDel = m_header.next;    // �������h���ĽY�c

            m_header.next = toDel->next;    // �Ƅ��^�Y�c�� next ָ�, ָ����һ���Y�c

            m_length--;

            destroy(toDel);   // ጷŵ�0̖�Y�c
        }


    }

    virtual bool move(int i, int step = 1) // i ��Y�c����, ��0�_ʼ������.
    {
        bool ret = (0 <= i) && (i < m_length) && (step > 0);

        if( ret )
        {
            m_current = position(i)->next; // ��λ���� i ���ڵĽY�c̎
            m_step = step; // �����Θ�ÿ���ƄӵĽY�c��Ŀ
        }

        return ret;
    }

    virtual bool end()
    {
        return (m_current == NULL);
    }

    virtual T current()
    {
        if( !end() ) // ( m_current != NUll ) = ( !end() )
        {
            return m_current->value;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No value at current position ...");
            // �Θ��ѽ����, �Q��Ԓ�f, ��v�ѽ��Y����, �@���r���Ҫ��ȥ�{�� current �ɆT����, �Ƿ�����.
        }
    }

    virtual bool next()
    {
        int i = 0; // ӛ��

        while((i < m_step) && !end()) // һ��Ҫѭ�h m_step ��, �K����ѭ�h���^�����Θ˲����
        {
            m_current = m_current->next;
            i++;
        }

        return (i == m_step); //�Д��Ƅ��Ƿ�ɹ�
    }


    ~LinkList()     //O(n)
    {
        clear();
    }
};

}

#endif // LINKLIST_H
