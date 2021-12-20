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
        �����e��ȫ�󌑁��ʾ, �@�N�^�c�����eҕ��һ�N���� (constant)�����H��Ҳ����ˡ�
        �@�N��������Ӳ��Ҏ��������һ�N׫�a�L��
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

    static BTreeNode<T>* NewNode() // ����ģʽ
    {
        BTreeNode<T>* ret = new BTreeNode<T>();

        if( ret != NULL )  // ����ѿռ�ɹ�
        {
            ret->m_flag = true;
        }

        return ret;
    }
};

}

#endif // BTREENODE_H
