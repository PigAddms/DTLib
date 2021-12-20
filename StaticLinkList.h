#ifndef STATICLINKLIST_H
#define STATICLINKLIST_H

#include "LinkList.h"

namespace DTLib
{

template< typename T, int N > // N ����ģ�兢��, �Á��x�A�����g��Сʹ�õ�.
class StaticLinkList : public LinkList<T>
{
protected:
    typedef typename LinkList<T>::Node Node;
    /*
        unsigned char m_space[sizeof(Node) * N]; error
        ��� Node �ͷ�ָ���� T �й�ϵ, ��˲���ֱ����������ʹ�� Node ���, ��Ӧ���� LinkList<T>::Node (ͨ�^���+������ֱ�����L�� Node)

        unsigned char m_space[sizeof(typename LinkList<T>::Node) * N];  ok
        ����� typename ����Ϊ��������֪������� Node �����ͻ��Ǿ�̬��Ա���������Ա������������ typename Ȼ������ typedef ����һ������.
    */

    struct SNode : public Node
    {
        void* operator new(unsigned int size, void* loc)
        {
            (void)size;
            //��������Ϊ����ʱ��û��ʹ�� size ������Ȼ������ C �����еı����ı��뷽ʽ.
            return loc;
        }
        /*
            new �����������d������Ҫ�ɂ�����, ���Ŀ������ָ���ăȴ����{�Ø��캯��
        */
    };

    unsigned char m_space[sizeof(SNode) * N];  // Ԥ���ռ�
    int m_used[N];   //Ԥ���ռ�ı������

    Node* create() // ����ȴ�, �Լ����� Node ����ĺ���.
    {
        SNode* ret = NULL;

        for(int i = 0; i < N; i++)
        {
            if( !m_used[i] ) // ��� i �@����Ԫ�]�б�ʹ��, �Ϳ��Ԍ��������ȥ
            {
                ret = reinterpret_cast<SNode*>(m_space) + i;
                /*
                  ��鸸��� Node �Ķ��x, ��ָ��͵� value �п�����һ���Ñ��Զ��x���͵Č���,
                  �Ǳ�Ȼ�ĕ����浽����Ć��}, �@Ȼ����Ҫ Node �Ę��캯���{��.

                  ret = reinterpret_cast<SNode*>(m_space) + i;
                  ����ȴ�, �����漰���캯�����{��, ߀��Ҫ��ָ���ȴ������{�Ø��캯��.
                */
                ret = new(ret)SNode();
                //��ָ���ռ�ret�ϵ���SNode��Ĺ��캯����
                m_used[i] = 1;
                break;
            }
        }

        return ret;
    }

    void destroy(Node* pn)
    {
        SNode* space = reinterpret_cast<SNode*>(m_space);
        SNode* psn = dynamic_cast<SNode*>(pn); // �ĸ��ָ��D�Q�����ָ�

        for(int i = 0; i<N; i++)
        {
            if( psn == (space + i))
            {
                m_used[i] = 0;
                psn->~SNode();
                break;
            }
        }
    }

public:
    StaticLinkList()
    {
        for(int i = 0; i < N; i++)
        {
            m_used[i] = 0;
        }
    }

    int capacity()
    {
        return N;
    }

    ~StaticLinkList()
    {
        this->clear();
    }
};

}

#endif // STATICLINKLIST_H
