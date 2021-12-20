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

    // 為了避開泛指類型 T 構造函數的調用
    mutable struct : public Object // 匿名類沒有繼承自 Object 就可能導致內存布局的不同
    {
        char reserved[sizeof(T)]; // 數組僅僅為了佔空間
        Node* next;
    }m_header;
    /*
        當定義單鏈表對象的時候, 必然會對成員來進行構造,
        這個時候構造頭結點對象的時候, 實現當中已經不可能調用任何的構造函數.

        這樣子做的話, 頭結點對象在內存布局上面, 和之前是沒有任何差異的.
        而差異僅僅是不管泛指類型 T 是什麼都不會調用構造函數.
    */

    int m_length;
    int m_step;
    Node* m_current;

    Node* position(int i) const     //O(n)   n 為泛指的表示法, 意思其實就是 i.
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

            if( m_current == toDel) // m_current 是不是已經指向即將被刪除的數據元素
            {
                m_current = toDel->next;
                // 移動 m_current, 使得指向即將被刪除的數據元素的下一個元素
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
                由於 get 函數是 const 成員函数, 我們在這個成員函數裡面
                不能修改任何成員變量的值.

                所以當我们要取頭結點 m_header 這個成員變量的地址, 这时编译器会认为
                我们有可能要修改成員變量的值, 所以就编译报错.

                因此我们在 m_header 这个指針加上 mutable 属性.
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

        while( node ) // 對鏈表進行遍歷, 用泛指的方法認為鏈表有 n 個數據元素 ==> O(n)
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
        while( m_header.next ) // 遍歷鏈表裡面的每一個數據元素 ==> O(n)
        {
            Node* toDel = m_header.next;    // 即將被刪除的結點

            m_header.next = toDel->next;    // 移動頭結點的 next 指針, 指向下一個結點

            m_length--;

            destroy(toDel);   // 釋放第0號結點
        }


    }

    virtual bool move(int i, int step = 1) // i 為結點的下標, 從0開始數數的.
    {
        bool ret = (0 <= i) && (i < m_length) && (step > 0);

        if( ret )
        {
            m_current = position(i)->next; // 定位到下標 i 所在的結點處
            m_step = step; // 保存游標每次移動的結點數目
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
            // 游標已經為空, 換句話說, 遍歷已經結束了, 這個時候非要再去調用 current 成員函數, 非法操作.
        }
    }

    virtual bool next()
    {
        int i = 0; // 記數

        while((i < m_step) && !end()) // 一共要循環 m_step 次, 並且在循環的過程中游標不為空
        {
            m_current = m_current->next;
            i++;
        }

        return (i == m_step); //判斷移動是否成功
    }


    ~LinkList()     //O(n)
    {
        clear();
    }
};

}

#endif // LINKLIST_H
