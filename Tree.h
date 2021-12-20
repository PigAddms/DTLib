#ifndef TREE_H
#define TREE_H

#include "TreeNode.h"
#include "SharedPointer.h"

namespace DTLib
{

template < typename T >
class Tree : public Object
{
protected:
    TreeNode<T>* m_root;

    // 禁止兩棵樹之間進行相互的複製.
    Tree(const Tree<T>&);
    Tree<T>& operator = (const Tree<T>&);

public:
    Tree()
    {
        m_root = NULL;
    }

    virtual bool insert(TreeNode<T>* node) = 0;
    virtual bool insert(const T& value, TreeNode<T>* parent) = 0;
    virtual SharedPointer< Tree<T> > remove(const T& value) = 0;
    virtual SharedPointer< Tree<T> > remove(TreeNode<T>* node) = 0;
    /*
        刪除操作刪除的是一個結點.

        刪除結點之後, 它後續的結點該如何處理?
        處理的方式有很多種, 沒有唯一正確的處理方式, 只有比較合理的處理方式.

        唐老師個人認為比較簡單的處理方式就是,刪除結點之後, 它後續的結點也要全部刪除.

        這樣子做484簡單粗暴了一點?
        是的, 所以在設計的時候做了一個折衷的方案.
        如果要刪除D結點, 那麼就將D結點所代表的子樹從原來的樹中刪除,
        刪除之後不將子樹銷毀並將子樹返回, 返回之後就有機會對裡面的元素做進一步的操作了.

        因此remove操作返回的是一棵樹, 並且這棵樹是由SharedPointer智能指針所指向的.
    */
    virtual TreeNode<T>* find(const T& value) const = 0;
    virtual TreeNode<T>* find(TreeNode<T>* node) const = 0;
    virtual TreeNode<T>* root() const = 0;
    virtual int degree() const = 0;
    virtual int count() const = 0;
    virtual int height() const = 0;
    virtual void clear() = 0;
    virtual bool begin() = 0;
    virtual bool end() = 0;
    virtual bool next() = 0;
    virtual T current() = 0;
};

}

#endif // TREE_H
