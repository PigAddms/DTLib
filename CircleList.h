#ifndef CIRCLELIST_H
#define CIRCLELIST_H

#include "LinkList.h"

namespace DTLib
{

template < typename T >
class CircleList : public LinkList<T>
{
protected:
    typedef typename LinkList<T>::Node Node;
    // Node 為父類裡面這個 typename LinkList<T>::Node 類型的別名

    int mod(int i) const    //O(1)
    {
        return (this->m_length == 0) ? 0 : (i % this->m_length);
        // 不能对 0 做取餘的操作 + 歸一化 i 的值
    }

    Node* last() const // last 函數获取指向尾結點的指针     //O(n)
    {
        return this->position(this->m_length - 1)->next;
        /*
            position 返回 i-1 个节点，再取 next 可以获取最后一个节点的指针.

            這個是從鏈表的思考方式來找到尾結點, 只有找到一個結點, 才能從這個結點找到下一個結點.
        */

        //return this->position(this->m_length);
        /*
            這樣寫的思考方式是從函數的角度來找到尾結點, 因為 position(i) 返回的是第 i 個結點的地址.
            (這裡的下標起始為 1, 2 , 3, ..., 並不是從 0 開始數數.)
        */

    }

    void last_to_first() const    //将链表首尾相连     //O(n)
    {
        last()->next = this->m_header.next;
        // 尾結點的 next 指針指向首結點
    }

public:
    bool insert(int i, const T& e)  //O(1) + O(n) + O(n) ==> O(2n+1) ==> O(n)
    {
        bool ret = true;

        i = i % (this->m_length + 1); // 歸一化 i 的值       //O(1)

        ret = LinkList<T>::insert(i, e);        //O(n)
        /*
            使用父類的 insert 函數來實現子類的 insert 函數.
            這也是為什麼 i 需要歸一化的原因, 由於是循環鏈表, i 的值可能非常大.
        */

        /***** 以上是基於單鏈表來實現循環鏈表, 還要注意首尾相連. *****/

        if( ret && (i == 0) )
        {
            last_to_first();    //O(n)
        }
        /*
            由於在父類的 insert 函數裡面, 有 node->next = current->next; 因此,
            只要插入第一個結點的時候, 使用 last_to_first() 函數,
            接著再插入的結點中的 next 指針都是指向首結點.
        */

        return ret;
    }

    bool insert(const T& e)     //O(n)
    {
        return insert(this->m_length, e); // 調用重載的版本
    }

    bool remove(int i)      //O(n)
    {
        bool ret = true;

        i = mod(i);

        if(i == 0) // 特殊處理刪除位置為 0 的狀況       //O(n)
        {
            Node* toDel = this->m_header.next;
            // 即將刪除的結點就是首結點

            if( toDel != NULL )
            {
                this->m_header.next = toDel->next;
                this->m_length--;

                if( this->m_length > 0 ) // 在還有元素的情況下(除了首元素), 才能挪動指針.
                {
                    last_to_first();    //在刪除元素下標為 0 的情況, 最耗時的部分為 O(n).

                    if( this->m_current == toDel )
                    {
                        this->m_current = toDel->next;
                    }
                }
                else
                {
                    this->m_header.next = NULL;
                    this->m_current = NULL;
                }

                this->destroy(toDel);
                // 最後一步才銷毀首結點是為了異常安全
            }
            else
            {
                ret = false;
            }
        }
        else
        {
            ret = LinkList<T>::remove(i);   //O(n)
        }

        return ret;
    }

    bool set(int i, const T& e)     //O(n)
    {
        return LinkList<T>::set(mod(i), e);
    }

    T get(int i) const      //O(n)
    {
        return LinkList<T>::get(mod(i));
    }

    bool get(int i, T& e) const     //O(n)
    {
        return LinkList<T>::get(mod(i), e);
    }

    int find(const T& e) const      //O(n)
    {
        int ret = -1;

        /*
            last()->next = NULL;
            // 循環鏈表變成單鏈表

            ret = LinkList<T>::find(e);
            // 於是可以直接調用父類的 find 函數

            last_to_first();
            // 再恢復成循環鏈表

            但是这是有问题的, 如果 find() 中的比较操作符是比較類的對象(T value),
            那肯定要重載相等比較操作符，相等比較操作符可能會拋出異常.

            拋出異常就異常返回, 这里又没有 try-catch, 異常會接著返回,
            last_to_first() 就沒有機會被調用了, 则会造成循環链表變成單鏈表,
            顯然就改變循環鏈表的狀態, 所以就不是異常安全的.

            DTLib 在開發的時候已經聲明了, 在實現的過程當中不允許使用 try-catch 語句塊,
            因為需要更好的移植性.

            必須重新實現 find 函數
        */

        Node* slider = this->m_header.next;

        for(int i=0; i<this->m_length; i++)
        {
            if( slider->value == e )
            {
                ret = i;
                break;
            }

            slider = slider->next;
        }

        return ret;
    }

    void clear()        // O(n+1) ==> O(n)
    {
        /*
            if( this->m_length >0 ) // 如果當前循環鏈表裡面是有結點的
            {
                last()->next = NULL;
                LinkList<T>::clear();
            }

            不可以這樣子實現 clear 函數.
            clear 函數裡面如果發生了異常, 那麼當前的鏈表就不再是循環鏈表了.

            需要重新實現 clear 函數
        */

        while( this->m_length > 1)      // O(n)
        {
            remove(1);  // 每次循環總是刪除下標為1的元素, 不會隨著問題的規模增加而增加, O(1).
            /*
                但凡當前的循環鏈表裡面結點的個數是大於1,
                那麼就將下標為 1 的結點給刪除掉, 直到當前的結點只有一個.

                為什麼是 remove(1) 而不是 remove(0)呢?
                因為效率, remove(0) 會造成大量的移動指針效率低, 而 remove(1) 會走 else 分支,
                效率就有極大的提高.
            */
        }

        if( this->m_length == 1)        //O(1)
        {
            Node* toDel = this->m_header.next;
            // 接下來要刪除的是首結點

            this->m_header.next = NULL;
            this->m_length = 0;
            this->m_current = NULL;

            this->destroy(toDel);
        }
    }

    bool move(int i, int step)      // O(n)
    {
        return LinkList<T>::move(mod(i), step);
    }

    bool end()      // O(1)
    {
        return (this->m_length == 0) || (this->m_current == NULL);
    }
    /*
        myself:
            除非在 for 迴圈有操作使得 !end() 不為真, 否則會一直遍歷整個循環鏈表.
            這個狀況與單鏈表不同, 單鏈表只遍歷一次.
    */

    ~CircleList()       // O(n)
    {
        clear();
    }

    /*
      有可能用 CircleList 代替 LinkList 工作, 所以 LinkList 中的成员函数都要变成虚函数才可以.
    */
};

}


#endif // CIRCLELIST_H
