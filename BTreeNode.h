#ifndef BTREENODE_H
#define BTREENODE_H

#include "TreeNode.h"

namespace DTLib
{
enum BTNodePos
{
    ANY,
    LEFT,
    RIGHT,
    /*
        將列舉用全大寫來表示, 這種觀點將列舉視為一種常數 (constant)，實際上也是如此。
        這種寫法不是硬性規定，而是一種撰碼風格。
    */
};

template < typename T >
class BTreeNode : public TreeNode<T>
{
public:
    BTreeNode<T>* left;
    BTreeNode<T>* right;

    BTreeNode()
    {
        left = NULL;
        right = NULL;
    }

    static BTreeNode<T>* NewNode() // 工厂模式
    {
        BTreeNode<T>* ret = new BTreeNode<T>();

        if( ret != NULL )  // 申请堆空间成功
        {
            ret->m_flag = true;
        }

        return ret;
    }
};

}

#endif // BTREENODE_H
