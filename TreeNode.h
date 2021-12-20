#ifndef TREENODE_H
#define TREENODE_H

#include "Object.h"

namespace DTLib
{

template < typename T >
class TreeNode : public Object
{
protected:
    bool m_flag;

    // 樹結點對象是不能夠被複製的
    TreeNode(const TreeNode<T>&);
    TreeNode<T>& operator = (const TreeNode<T>&);
    /*
        子類對象在創建時會先調用父類的構造函數, 因此在父類裡面聲明保護的拷貝構造函數,
        可以避免從外部調用父類拷貝構造函數, 所以也就不能創建子類對象.

        從外部是不能調用保護的賦值操作符重載函數, 只有公有的函數才能從外部調用.
    */

    void* operator new(unsigned int size) throw()
    {
        return Object::operator new(size);
        // 具體的實現還是沿用頂層父類的實現
    }

public:
    T value;
    TreeNode<T>* parent;

    TreeNode()
    {
        m_flag = false;
        parent = NULL;
    }

    bool flag()
    {
        return m_flag;
    }

    virtual ~TreeNode() = 0;
};

template < typename T >
TreeNode<T>::~TreeNode(){ }
/*
    雖然析構函數已經是純虛函數了, 但是還是得給它提供一個空的函數體,
    否則的話, 可能編譯不過.
*/

}

#endif // TREENODE_H
