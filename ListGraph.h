#ifndef LISTGRAPH_H
#define LISTGRAPH_H

#include "Graph.h"
#include "LinkList.h"
#include "Exception.h"
#include "DynamicArray.h"

namespace DTLib
{

template < typename V, typename E >
class ListGraph : public Graph<V, E>
{
protected:
    struct Vertex : public Object   //定义顶点
    {
        V* data;
        LinkList< Edge<E> > edge;

        Vertex()
        {
            data = NULL;
        }
    };

    LinkList<Vertex*> m_list;   //真正的邻接链表
public:
    ListGraph(unsigned int n = 0)
    {
        for(unsigned int i=0; i<n; i++)
        {
            addVertex();
        }
    }

    int addVertex() // O(n)
    {
        int ret = -1;
        Vertex* v = new Vertex();

        if( v != NULL )
        {
            m_list.insert(v); // O(n)

            ret = m_list.length() - 1; //返回新增加的顶点的编号
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new vertex object ...");
        }

        return ret;
    }

    int addVertex(const V& value) // O(n)
    {
        int ret = addVertex(); // O(n)

        if( ret >= 0 ) // 增加頂點成功
        {
            setVertex(ret, value); // O(n)
        }

        return ret;
    }

    bool setVertex(int i, const V& value) // O(n)
    {
        int ret = ((0 <= i) && (i < vCount()));

        if( ret )
        {
            Vertex* vertex = m_list.get(i); // O(n)
            V* data = vertex->data;

            if( data == NULL )
            {
                data = new V();
            }

            if( data != NULL )
            {
                *data = value;

                vertex->data = data;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new vertex value ...");
            }
        }

        return ret;
    }

    V getVertex(int i) // O(n)
    {
        V ret;

        if( !getVertex(i, ret) )
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }

        return ret;
    }

    bool getVertex(int i, V& value) // O(n)
    {
        int ret = ((0 <= i) && (i < vCount()));

        if( ret )
        {
            Vertex* v = m_list.get(i); // O(n)

            if( v->data != NULL )
            {
                value = *(v->data);
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException, "No value assigned to this vertex ...");
            }
        }

        return ret;
    }

    void removeVertex() // O(n*n)
    {
        if( m_list.length() > 0 )
        {
            int index = m_list.length() - 1;
            // 要刪除的頂點編號
            Vertex* v = m_list.get(index); // O(n)

            if( m_list.remove(index) ) // 刪除鏈表的結點
            {
                // 看一看其它頂點的鄰接鏈表裡面是否有與要刪除的頂點有相關的邊, 如果有要刪除.
                for(int i=(m_list.move(0), 0); !m_list.end(); i++, m_list.next()) // O(n)
                {
                    int pos = m_list.current()->edge.find(Edge<E>(i, index)); //O(n)

                    if( pos >= 0 )
                    {
                        m_list.current()->edge.remove(pos);
                    }
                }

                // 鏈表結點中的數據元素是一個指向頂點的指針, 而頂點裡面又有一個指向數據元素的指針, 因此要多刪除2次
                delete v->data;
                delete v;
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No vertex in current graph ...");
        }
    }

    SharedPointer< Array<int> > getAdjacent(int i) // O(n)
    {
        DynamicArray<int>* ret = NULL;

        if( (0 <= i) && (i < vCount()) )
        {
            Vertex* vertex = m_list.get(i); // O(n)

            ret = new DynamicArray<int>(vertex->edge.length());

            if( ret != NULL )
            {
                for(int k=(vertex->edge.move(0), 0); !vertex->edge.end(); k++, vertex->edge.next()) //O(n)
                {
                    ret->set(k, vertex->edge.current().e);
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create ret object ...");
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid...");
        }

        return ret;
    }

    bool isAdjacent(int i, int j)
    {
        return (0 <= i) && (i < vCount()) && (0 <= j) && (j < vCount()) && (m_list.get(i)->edge.find(Edge<E>(i, j)) >= 0);
    }

    E getEdge(int i, int j) // O(n)
    {
        E ret;

        if( !getEdge(i, j, ret) )
        {
            THROW_EXCEPTION(InvalidParameterException, "Edge <i, j> is invalid ...");
        }

        return ret;
    }

    bool getEdge(int i, int j, E& value) // O(n)
    {
        int ret = ( (0 <= i) && (i < vCount()) &&
                    (0 <= j) && (j < vCount()) );

        if( ret )
        {
            Vertex* vertex = m_list.get(i); // O(n)
            int pos = vertex->edge.find(Edge<E>(i, j)); // O(n)

            if( pos >= 0 )
            {
                value = vertex->edge.get(pos).data; // O(n)
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException, "No value assigned to this edge ...");
            }
        }

        return ret;
    }

    bool setEdge(int i, int j, const E& value) // O(n)
    {
        int ret = ( (0 <= i) && (i < vCount()) &&
                    (0 <= j) && (j < vCount()) );

        if( ret )
        {
            Vertex* vertex = m_list.get(i); // O(n)
            int pos = vertex->edge.find(Edge<E>(i, j)); // O(n)

            if( pos >= 0 )
            {
                ret = vertex->edge.set(pos, Edge<E>(i, j, value));
            }
            else
            {
                ret = vertex->edge.insert(0, Edge<E>(i, j, value));
            }
        }

        return ret;
    }

    bool removeEdge(int i, int j)   // O(n)
    {
        int ret = ( (0 <= i) && (i < vCount()) &&
                    (0 <= j) && (j < vCount()) );

        if( ret )
        {
            Vertex* vertex = m_list.get(i); // O(n)
            int pos = vertex->edge.find(Edge<E>(i, j)); // O(n)

            if( pos >= 0 )
            {
                ret = vertex->edge.remove(pos); // O(n)
            }
        }

        return ret;
    }

    int vCount() // O(1)
    {
        return m_list.length(); // O(1)
    }

    int eCount() // O(n)
    {
        int ret = 0;

        for(m_list.move(0); !m_list.end(); m_list.next())
        {
            ret += m_list.current()->edge.length();
        }

        return ret;
    }

    int ID(int i) // O(n*n)
    {
        int ret = 0;

        if( (0 <= i) && (i < vCount()) )
        {
            for(m_list.move(0); !m_list.end(); m_list.next()) // O(n)
            {
                LinkList< Edge<E> >& edge = m_list.current()->edge;
                // 引用為當前頂點的鄰接鏈表的別名

                for(edge.move(0); !edge.end(); edge.next()) // O(n)
                {
                    if( edge.current().e == i )
                    {
                        ret++;
                        break;
                    }
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }

        return ret;
    }

    int OD(int i) // O(n)
    {
        int ret = 0;

        if( (0 <= i) && (i < vCount()) )
        {
            ret = m_list.get(i)->edge.length(); // O(n)
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }

        return ret;
    }

    ~ListGraph()
    {
        while( m_list.length() > 0 )
        {
            Vertex* toDel = m_list.get(0);

            m_list.remove(0);

            delete toDel->data;
            delete toDel;
        }
    }
};

}

#endif // LISTGRAPH_H
