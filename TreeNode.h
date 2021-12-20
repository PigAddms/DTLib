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

    // ��Y�c�����ǲ��܉��}�u��
    TreeNode(const TreeNode<T>&);
    TreeNode<T>& operator = (const TreeNode<T>&);
    /*
        ������ڄ����r�����{�ø�Ę��캯��, ����ڸ���e�������o�Ŀ�ؐ���캯��,
        ���Ա�����ⲿ�{�ø��ؐ���캯��, ����Ҳ�Ͳ��܄��������.

        ���ⲿ�ǲ����{�ñ��o���xֵ���������d����, ֻ�й��еĺ������܏��ⲿ�{��.
    */

    void* operator new(unsigned int size) throw()
    {
        return Object::operator new(size);
        // ���w�Č��F߀������플Ӹ�Č��F
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
    �mȻ���������ѽ��Ǽ�̓������, ����߀�ǵýo���ṩһ���յĺ����w,
    ��t��Ԓ, ���ܾ��g���^.
*/

}

#endif // TREENODE_H
