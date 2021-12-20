#ifndef MATRIXGRAPH_H
#define MATRIXGRAPH_H

#include "Graph.h"
#include "Exception.h"
#include "DynamicArray.h"

namespace DTLib
{

// N是圖中頂點個數, V是顶点的數據元素类型, E是边的數據元素类型
template < int N, typename V, typename E >
class MatrixGraph : public Graph<V, E>
{
protected:
    V* m_vertexes[N];
    E* m_edges[N][N];
    int m_eCount;
public:
    MatrixGraph()
    {
        for(int i=0; i<vCount(); i++)
        {
            m_vertexes[i] = NULL;

            for( int j=0; j<vCount(); j++)
            {
                m_edges[i][j] = NULL;
            }
        }

        m_eCount = 0;
    }

    V getVertex(int i) // O(1)
    {
        V ret;

        if( !getVertex(i, ret) )
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
            // 調用 getVertex(int i, V& value) 返回 false, 意味著參數不合法.
        }

        return ret;
    }

    bool getVertex(int i, V& value) // O(1)
    {
        bool ret = ( (0 <= i) && (i < vCount()) );

        if( ret )
        {
            if( m_vertexes[i] != NULL )
            {
                value = *(m_vertexes[i]);
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException, "No value assigned to this vertex ...");
                // 根本沒有數據元素與頂點相關聯.
            }
        }

        return ret;
    }
    /*
        O(1)
        沒有任何的循環, 換句話說, 不會因為這個圖規模的增加,
        這個操作它所耗的時間就會增加.
    */

    bool setVertex(int i, const V& value) // O(1)
    {
        bool ret = ( (0 <= i) && (i < vCount()) );

        /*
        if( ret )
        {
            if( m_vertexes[i] == NULL )
            {
                m_vertexes[i] = new V();
            }

            if( m_vertexes[i] != NULL )
            {
                *(m_vertexes[i]) = data;

                假設在做賦值操作的時候產生異常, 意味著函數要異常返回,
                圖對象的內部狀態被改變, 編號為 i 的頂點不為空,
                並且它所關聯的值並不是一個合法的值.

                換句話說, 但凡這個函數出現異常, 圖對象就不能用了.
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to store new vertex value ...");
            }
        }
        */

        if( ret )
        {
            V* data = m_vertexes[i];
            // 為了異常安全, 使用中間變量.

            if( data == NULL )
            {
                data = new V();
            }

            if( data != NULL )
            {
                *data = value;
                // 假設這裡出現異常, 圖對象的狀態並沒有被改變.

                m_vertexes[i] = data;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to store new vertex value ...");
            }
        }

        return ret;
    }

    //获取i的邻接顶点編號
    SharedPointer< Array<int> > getAdjacent(int i) // O(n)
    {
        DynamicArray<int>* ret = NULL;

        if( (0 <= i) && (i < vCount()) )
        {
            int n = 0; // 計數, 與目標頂點所鄰接的頂點個數

            for(int j=0; j<vCount(); j++)
            {
                if( m_edges[i][j] != NULL )
                {
                    n++;
                }
            }

            ret = new DynamicArray<int>(n);

            if( ret != NULL )
            {
                for(int j=0, k=0; j<vCount(); j++)
                {
                    if( m_edges[i][j] != NULL )
                    {
                        ret->set(k++, j);
                    }
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new ret object ...");
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }

        return ret;
    }
    /*
        O(n)
        耗時與圖的規模相關, 頂點越多, 這個函數越耗時.
    */

    bool isAdjacent(int i, int j)
    {
        return (0 <= i) && (i < vCount()) && (0 <= j) && (j < vCount()) && (m_edges[i][j] != NULL);
    }

    E getEdge(int i, int j) // O(1)
    {
        E ret;

        if( !getEdge(i, j, ret) )
        {
            THROW_EXCEPTION(InvalidParameterException, "Index <i,j> is invalid ...");
        }

        return ret;
    }

    bool getEdge(int i, int j, E& value) // O(1)
    {
        bool ret = ((0 <= i) && (i < vCount()) &&
                    (0 <= j) && (j < vCount()) );

        if( ret )
        {
            if( m_edges[i][j] != NULL )
            {
                value = *(m_edges[i][j]);
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException, "No value assigned to this edge ...");
            }
        }

        return ret;
    }

    bool setEdge(int i, int j, const E& value) // O(1)
    {
        bool ret = ((0 <= i) && (i < vCount()) &&
                    (0 <= j) && (j < vCount()) );

        if( ret )
        {
            E* ne = m_edges[i][j]; // 中間變量

            if( ne == NULL )
            {
                //i和j之间没有边，现在要设置一条边
                ne = new E();

                if( ne != NULL )
                {
                    *ne = value;

                    m_edges[i][j] = ne;

                    m_eCount++;
                }
                else
                {
                    THROW_EXCEPTION(NoEnoughMemoryException, "No memory to store new edge value ...");
                }
            }
            else
            {
                *ne = value;
            }
        }

        return ret;
    }

    bool removeEdge(int i, int j) // O(1)
    {
        bool ret = ((0 <= i) && (i < vCount()) &&
                    (0 <= j) && (j < vCount()) );

        if( ret )
        {
            E* toDel = m_edges[i][j]; // 中間變量

            m_edges[i][j] = NULL;

            if( toDel != NULL )
            {
                m_eCount--;

                delete toDel;
            }
        }

        return ret;
    }

    int vCount() // O(1)
    {
        return N;
    }

    int eCount() // O(1)
    {
        return m_eCount;
    }

    int OD(int i) // O(n)
    {
        // 鄰接矩陣裡面, 對應的列當中不為0的數據元素個數, 就代表對應頂點的出度

        int ret = 0;

        if( (0 <= i) && (i < vCount()) )
        {
            // 数邻接矩阵对应的列中有多少个不为0的个数
            for(int j=0; j<vCount(); j++)
            {
                if( m_edges[i][j] != NULL )
                {
                    ret++;
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }

        return ret;
    }

    int ID(int i) // O(n)
    {
        // 鄰接矩陣裡面, 對應的行當中不為0的數據元素個數, 就代表對應頂點的入度

        int ret = 0;

        if( (0 <= i) && (i < vCount()) )
        {
            // 数邻接矩阵对应的行中有多少个不为0的个数
            for(int j = 0; j < vCount(); j++)
            {
                if( m_edges[j][i] != NULL )
                {
                    ret++;
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid...");
        }

        return ret;
    }

    ~MatrixGraph()
    {
        for(int i=0; i<vCount(); i++)
        {
            for( int j=0; j<vCount(); j++)
            {
                delete m_edges[i][j];
            }

            delete m_vertexes[i];
        }
    }
};

}

#endif // MATRIXGRAPH_H
