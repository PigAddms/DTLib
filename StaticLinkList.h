#ifndef STATICLINKLIST_H
#define STATICLINKLIST_H

#include "LinkList.h"

namespace DTLib
{

template< typename T, int N > // N 整型模板參數, 用來定義預留空間大小使用的.
class StaticLinkList : public LinkList<T>
{
protected:
    typedef typename LinkList<T>::Node Node;
    /*
        unsigned char m_space[sizeof(Node) * N]; error
        由於 Node 和泛指类型 T 有关系, 因此不能直接在子类中使用 Node 類型, 而应该是 LinkList<T>::Node (通過類名+作用域分辨符來訪問 Node)

        unsigned char m_space[sizeof(typename LinkList<T>::Node) * N];  ok
        这里的 typename 是因为编译器不知道这里的 Node 是类型还是静态成员变量，所以编译器建议加上 typename 然后又用 typedef 来简化一个类型.
    */

    struct SNode : public Node
    {
        void* operator new(unsigned int size, void* loc)
        {
            (void)size;
            //这里是因为编译时候，没有使用 size 参数，然后加入的 C 语言中的暴力的编译方式.
            return loc;
        }
        /*
            new 操作符的重載函數需要兩個參數, 因為目的是在指定的內存上調用構造函數
        */
    };

    unsigned char m_space[sizeof(SNode) * N];  // 预留空间
    int m_used[N];   //预留空间的标记数组

    Node* create() // 分配內存, 以及創建 Node 對象的函數.
    {
        SNode* ret = NULL;

        for(int i = 0; i < N; i++)
        {
            if( !m_used[i] ) // 如果 i 這個單元沒有被使用, 就可以將它分配出去
            {
                ret = reinterpret_cast<SNode*>(m_space) + i;
                /*
                  因為父類當中 Node 的定義, 泛指類型的 value 有可能是一個用戶自定義類類型的對象,
                  那必然的會牽涉到構造的問題, 顯然的需要 Node 的構造函數調用.

                  ret = reinterpret_cast<SNode*>(m_space) + i;
                  分配內存, 不會涉及構造函數的調用, 還需要在指定內存上面調用構造函數.
                */
                ret = new(ret)SNode();
                //在指定空间ret上调用SNode类的构造函数。
                m_used[i] = 1;
                break;
            }
        }

        return ret;
    }

    void destroy(Node* pn)
    {
        SNode* space = reinterpret_cast<SNode*>(m_space);
        SNode* psn = dynamic_cast<SNode*>(pn); // 從父類指針轉換到子類指針

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
