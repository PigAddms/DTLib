#ifndef GTREE_H
#define GTREE_H

#include "Tree.h"
#include "GTreeNode.h"
#include "Exception.h"
#include "LinkQueue.h"


namespace DTLib
{

template < typename T >
class GTree : public Tree<T>
{
protected:
    LinkQueue<GTreeNode<T>*> m_queue;

    GTreeNode<T>* find(GTreeNode<T>* node, const T& value) const
    {
        GTreeNode<T>* ret = NULL;
        // 定義返回值變量, 查找不到的時候返回空.

        if( node != NULL )
        {
            if( node->value == value )
            {
                return node;
            }
            else
            {
                for(node->child.move(0); !node->child.end() && (ret == NULL); node->child.next()) //
                {
                    /*
                        ret == NULL, 判斷返回值是否為空, 為空就代表還沒找到.

                        move(0)如果操作失敗, 會返回0.
                    */

                    ret = find(node->child.current(), value);
                    /*
                        在這一個指針 node->child.current() 作為根結點的樹中,
                        查找 value 這一個成員.

                        如果查找成功了, ret 的值肯定不為空,
                        for 循環就可以不用再繼續了, 於是函數就應該返回了.
                    */

                }

            }
        }

        return ret;
    }

    GTreeNode<T>* find(GTreeNode<T>* node, GTreeNode<T>* obj) const
    {
        GTreeNode<T>* ret = NULL;

        if( node == obj )
        {
            return node;
        }
        else
        {
            if( node != NULL ) // 是否為空樹的判斷, 可以跟查找值的find函數比較一下, 意思相同, 寫法不同.
            {
                for( node->child.move(0); !node->child.end() && (ret == NULL); node->child.next() )
                {
                    ret = find(node->child.current(), obj);
                }
            }
        }

        return ret;
    }

    void free(GTreeNode<T>* node) // node 指針指向需要清除的樹的根結點
    {
        if( node != NULL )
        {
            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                free(node->child.current());
            }
            /*
                如果樹中只有根結點, 此時清除所有結點, 則會進來 else 分支, node->child.move(0),
                由於只有根結點, 鏈表此時沒有存放任何子結點, 因此 move(0) 會返回0, 而0則會使得下次
                遞歸調用函數直接返回.

                因此如果只有根結點的情況, 遞歸出口就會還是 node == null;
            */

            if( node->flag() )
            {
                delete node;
            }
            /*
                只有堆中的對象需要被釋放, 如果不是堆中的對象,
                生命週期是不需要我們在代碼裡面管理的.

                我們不能夠簡單暴力的 delete node 所指向的結點, 如果結點來自於
                棧區或是全局數據區, 要 delete 來自於這兩個存儲區的結點,
                就會導致程序的不穩定.
            */
        }
    }

    void remove(GTreeNode<T>* node, GTree<T>*& ret)
    {
        ret = new GTree<T>();

        if( ret == NULL )
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create tree...");
        }
        else
        {
            if( root() == node )
            {
                this->m_root = NULL;
            }
            else
            {
                LinkList<GTreeNode<T>*>& child = dynamic_cast<GTreeNode<T>*>(node->parent)->child;
                // 定義一個引用, 這個引用是 node 的父結點中的孩子鏈表的別名.

                child.remove(child.find(node));
                // 將當前要刪除的結點, 從它父結點給刪除掉.

                node->parent = NULL;
            }

            ret->m_root = node;
        }
    }

    int count(GTreeNode<T>* node) const // 因為 const 函數只能調用 const 函數.
    {
        int ret = 0;

        if( node != NULL )
        {
            ret = 1; // node 不為空, 代表至少的已經有一個結點.

            // 如果 node 没有孩子，则 for 循環不执行.
            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                ret += count(node->child.current());
                // 遍歷這些孩子做累加, 累加上這些結點所代表的子樹的結點數.
            }
        }

        return ret;
    }

    int height(GTreeNode<T>* node) const
    {
        int ret = 0;

        if( node != NULL )
        {
            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                int h = height(node->child.current());

                if( ret < h ) // 看看是否比先前高度要大
                {
                    ret = h;
                    // 如果說比之前的高度要大, 那它就是當前的最大高度了.
                }
            }

            ret = ret + 1;
        }

        return ret;
    }

    int degree(GTreeNode<T>* node) const
    {
        int ret = 0;

        if( node != NULL )
        {
            ret = node->child.length();  // 根节点的度数

            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                int d = degree(node->child.current());

                if( ret < d )
                {
                    ret = d;
                }
            }
        }

        return ret;
    }

public:
    GTree()
    {

    }

    bool insert(TreeNode<T>* node)
    {
        bool ret = true;

        // 不可能在一個樹裡面插入空結點.
        if( node != NULL )
        {
            // 如果當前的樹是一棵空樹, 那麼插入的新結點就是根結點.
            if( this->m_root == NULL )
            {
                node->parent = NULL;
                this->m_root = node;
            }
            else // 如果當前的樹不是一棵空樹
            {
                GTreeNode<T>* np = find(node->parent);
                // 找一找指定的父結點是否存在於當前的樹中

                if( np != NULL )
                {
                    // 需要考慮是否會重複插入, 只有當要插入的新結點不存在樹中, 才插入新結點.
                    GTreeNode<T>* n = dynamic_cast<GTreeNode<T>*>(node);

                    if( np->child.find(n) < 0 ) // 小於0代表新結點 node 確實沒有在當前樹中.
                    {
                        np->child.insert(n); // 插入到父结点的子结点链表里面去
                    }
                }
                else
                {
                    THROW_EXCEPTION(InvalidParameterException, "Invalid parent tree node...");
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Parameter node cannot be NULL ...");
        }

        return ret;
    }

    bool insert(const T& value, TreeNode<T>* parent)
    {
        bool ret = true;

        GTreeNode<T>* node = GTreeNode<T>::NewNode();

            if( node != NULL )
            {
                node->value = value;
                node->parent = parent;

                insert(node);
                /*
                    這裡不是遞歸調用, 是調用 bool insert(TreeNode<T>* node),
                    參數不相同的.
                */
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new tree node ...");
            }

        return ret;
    }

    SharedPointer< Tree<T> > remove(const T& value)
    {
        GTree<T>* ret = NULL;

        GTreeNode<T>* node = find(value);

        if( node == NULL )
        {
            THROW_EXCEPTION(InvalidParameterException, "Can not find the node via parameter value ...");
        }
        else
        {
            remove(node, ret);

            m_queue.clear();
            // 如果從樹裡面刪除了結點, 就將隊列清除.
        }

        return ret;

        /*
            為何智能指針所指向的類型為 Tree 而非 GTree?
            因為指向父類 Tree 的智能指針能夠接受各種子類對象的地址,
            並且由於多態的關係, 智能指針能夠使用子類對象裡面實現的函數.
        */
    }

    SharedPointer< Tree<T> > remove(TreeNode<T>* node)
    {
        GTree<T>* ret = NULL;

        node = find(node);

        if( node == NULL )
        {
            THROW_EXCEPTION(InvalidParameterException, "Parameter node is invalid ...");
        }
        else
        {
            remove(dynamic_cast<GTreeNode<T>*>(node), ret);
            /*
                node 的類型是 TreeNode, 並不是 GTreeNode,
                所以要將指向父類的指針, 轉換到指向子類的指針.
            */

            m_queue.clear();
            // 如果從樹裡面刪除了結點, 就將隊列清除.
        }

        return ret;
    }

    GTreeNode<T>* find(const T& value) const
    {
        return find(root(), value);
    }
    /*
        函數重寫的返回值類型從 TreeNode -> GTreeNode 這樣子合法嗎?
        從C++語言的角度來看, 是合法的, 因為賦值兼容性原則.
    */

    GTreeNode<T>* find(TreeNode<T>* node) const
    {
        return find(root(), dynamic_cast<GTreeNode<T>*>(node));
    }

    GTreeNode<T>* root() const
    {
        return dynamic_cast<GTreeNode<T>*>(this->m_root);
        // Gtree 的根結點指針, 真正的類型顯然是 GTreeNode.
    }

    int degree() const
    {
        return degree(root());
    }
    int count() const
    {
        return count(root());
    }

    int height() const
    {
        return height(root());
    }

    void clear()
    {
        free(root());

        this->m_root = NULL;

        m_queue.clear();
    }

    bool begin()
    {
        bool ret = ( root() != NULL );

        if( ret )
        {
            m_queue.clear();
            /*
                萬一上一次的遍歷還沒結束, 又調用了一次 begin() 怎麼辦?
                先 clear(), 保證 begin() 被調用了之後, 在隊列裡面就只有根結點.
            */

            m_queue.add(root());
        }

        return ret;
    }

    bool end()
    {
        return (m_queue.length() == 0);
    }

    bool next()
    {
        bool ret = (m_queue.length() > 0);
        // 必須有數據元素才能夠移動游標

        if( ret )
        {
            GTreeNode<T>* node = m_queue.front();
            // node 指向的是隊頭的數據元素, 隊列中結點的數據元素就是指向樹結點的指針.

            m_queue.remove();
            //将对头元素出队, 相当于移动游标, 指向下一個結點.

            // 將出隊列的那一個樹結點的孩子壓入隊列裡面, 出隊列的結點被 node 指向.
            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                m_queue.add(node->child.current());
            }
        }

        return ret;
    }

    // 返回當前游標所指向的結點中的數據元素
    T current()
    {
        if( !end() )  // 遍历的过程当中调用current函数才有意义
        {
            return m_queue.front()->value;
            /*
                將游標所指向的結點中的數據元素返回. (游標觀點, 此時的結點是樹結點)
            */
            // 隊列存儲的結點中的數據元素是樹結點, 樹結點中存儲真正的數據元素.
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No value at current position...");
        }
        /*
            這個判斷的意思就是說, 只有在遍歷沒結束的時候, current() 才有意義,
            否則是沒有意義的.

            如果遍歷都沒有開始, 游標誰也沒指向, 又如果遍歷結束了, 游標也誰都有指向.
        */
    }

    ~GTree()
    {
        clear();
    }
};

}

#endif // GTREE_H
