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
        // ���x����ֵ׃��, ���Ҳ����ĕr�򷵻ؿ�.

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
                        ret == NULL, �Д෵��ֵ�Ƿ���, ��վʹ���߀�]�ҵ�.

                        move(0)�������ʧ��, ������0.
                    */

                    ret = find(node->child.current(), value);
                    /*
                        ���@һ��ָ� node->child.current() ������Y�c�Ę���,
                        ���� value �@һ���ɆT.

                        ������ҳɹ���, ret ��ֵ�϶������,
                        for ѭ�h�Ϳ��Բ������^�m��, ��Ǻ����͑�ԓ������.
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
            if( node != NULL ) // �Ƿ��՘���Д�, ���Ը�����ֵ��find�������^һ��, ��˼��ͬ, ������ͬ.
            {
                for( node->child.move(0); !node->child.end() && (ret == NULL); node->child.next() )
                {
                    ret = find(node->child.current(), obj);
                }
            }
        }

        return ret;
    }

    void free(GTreeNode<T>* node) // node ָ�ָ����Ҫ����Ę�ĸ��Y�c
    {
        if( node != NULL )
        {
            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                free(node->child.current());
            }
            /*
                �������ֻ�и��Y�c, �˕r������нY�c, �t���M�� else ��֧, node->child.move(0),
                ���ֻ�и��Y�c, 朱�˕r�]�д���κ��ӽY�c, ��� move(0) ������0, ��0�t��ʹ���´�
                �f�w�{�ú���ֱ�ӷ���.

                ������ֻ�и��Y�c����r, �f�w���ھ͕�߀�� node == null;
            */

            if( node->flag() )
            {
                delete node;
            }
            /*
                ֻ�ж��еČ�����Ҫ��ጷ�, ������Ƕ��еČ���,
                �����L���ǲ���Ҫ�҂��ڴ��a�e������.

                �҂����܉򺆆α����� delete node ��ָ��ĽY�c, ����Y�c�����
                ���^����ȫ�֔����^, Ҫ delete ������@�ɂ��惦�^�ĽY�c,
                �͕����³���Ĳ�����.
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
                // ���xһ������, �@�������� node �ĸ��Y�c�еĺ���朱�Ąe��.

                child.remove(child.find(node));
                // ����ǰҪ�h���ĽY�c, �������Y�c�o�h����.

                node->parent = NULL;
            }

            ret->m_root = node;
        }
    }

    int count(GTreeNode<T>* node) const // ��� const ����ֻ���{�� const ����.
    {
        int ret = 0;

        if( node != NULL )
        {
            ret = 1; // node �����, �������ٵ��ѽ���һ���Y�c.

            // ��� node û�к��ӣ��� for ѭ�h��ִ��.
            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                ret += count(node->child.current());
                // ��v�@Щ�������ۼ�, �ۼ����@Щ�Y�c��������Ә�ĽY�c��.
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

                if( ret < h ) // �����Ƿ����ǰ�߶�Ҫ��
                {
                    ret = h;
                    // ����f��֮ǰ�ĸ߶�Ҫ��, �������Ǯ�ǰ�����߶���.
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
            ret = node->child.length();  // ���ڵ�Ķ���

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

        // ��������һ�����e�����սY�c.
        if( node != NULL )
        {
            // �����ǰ�Ę���һ�ÿ՘�, ���N������½Y�c���Ǹ��Y�c.
            if( this->m_root == NULL )
            {
                node->parent = NULL;
                this->m_root = node;
            }
            else // �����ǰ�Ę䲻��һ�ÿ՘�
            {
                GTreeNode<T>* np = find(node->parent);
                // ��һ��ָ���ĸ��Y�c�Ƿ����춮�ǰ�Ę���

                if( np != NULL )
                {
                    // ��Ҫ���]�Ƿ�����}����, ֻ�Ю�Ҫ������½Y�c�����ژ���, �Ų����½Y�c.
                    GTreeNode<T>* n = dynamic_cast<GTreeNode<T>*>(node);

                    if( np->child.find(n) < 0 ) // С�0�����½Y�c node �_���]���ڮ�ǰ����.
                    {
                        np->child.insert(n); // ���뵽�������ӽ����������ȥ
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
                    �@�e�����f�w�{��, ���{�� bool insert(TreeNode<T>* node),
                    ��������ͬ��.
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
            // ����Ę��e��h���˽Y�c, �͌�������.
        }

        return ret;

        /*
            �������ָ���ָ�����͞� Tree ���� GTree?
            ���ָ��� Tree ������ָ��܉���ܸ��N�����ĵ�ַ,
            �K����춶��B���P�S, ����ָ��܉�ʹ��������e�挍�F�ĺ���.
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
                node ������� TreeNode, �K���� GTreeNode,
                ����Ҫ��ָ���ָ�, �D�Q��ָ�����ָ�.
            */

            m_queue.clear();
            // ����Ę��e��h���˽Y�c, �͌�������.
        }

        return ret;
    }

    GTreeNode<T>* find(const T& value) const
    {
        return find(root(), value);
    }
    /*
        �����،��ķ���ֵ��͏� TreeNode -> GTreeNode �@���ӺϷ���?
        ��C++�Z�ԵĽǶȁ�, �ǺϷ���, ����xֵ������ԭ�t.
    */

    GTreeNode<T>* find(TreeNode<T>* node) const
    {
        return find(root(), dynamic_cast<GTreeNode<T>*>(node));
    }

    GTreeNode<T>* root() const
    {
        return dynamic_cast<GTreeNode<T>*>(this->m_root);
        // Gtree �ĸ��Y�cָ�, ����������@Ȼ�� GTreeNode.
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
                �fһ��һ�εı�v߀�]�Y��, ���{����һ�� begin() ���N�k?
                �� clear(), ���C begin() ���{����֮��, ������e���ֻ�и��Y�c.
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
        // ����Д���Ԫ�ز��܉��Ƅ��Θ�

        if( ret )
        {
            GTreeNode<T>* node = m_queue.front();
            // node ָ�������^�Ĕ���Ԫ��, ����нY�c�Ĕ���Ԫ�ؾ���ָ���Y�c��ָ�.

            m_queue.remove();
            //����ͷԪ�س���, �൱���ƶ��α�, ָ����һ���Y�c.

            // ������е���һ����Y�c�ĺ��Ӊ�������e��, ����еĽY�c�� node ָ��.
            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                m_queue.add(node->child.current());
            }
        }

        return ret;
    }

    // ���خ�ǰ�Θ���ָ��ĽY�c�еĔ���Ԫ��
    T current()
    {
        if( !end() )  // �����Ĺ��̵��е���current������������
        {
            return m_queue.front()->value;
            /*
                ���Θ���ָ��ĽY�c�еĔ���Ԫ�ط���. (�Θ��^�c, �˕r�ĽY�c�ǘ�Y�c)
            */
            // ��д惦�ĽY�c�еĔ���Ԫ���ǘ�Y�c, ��Y�c�д惦�����Ĕ���Ԫ��.
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No value at current position...");
        }
        /*
            �@���Д����˼�����f, ֻ���ڱ�v�]�Y���ĕr��, current() �������x,
            ��t�Ǜ]�����x��.

            �����v���]���_ʼ, �Θ��lҲ�]ָ��, �������v�Y����, �Θ�Ҳ�l����ָ��.
        */
    }

    ~GTree()
    {
        clear();
    }
};

}

#endif // GTREE_H
