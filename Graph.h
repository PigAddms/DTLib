#ifndef GRAPH_H
#define GRAPH_H

#include "Object.h"
#include "SharedPointer.h"
#include "Array.h"
#include "DynamicArray.h"
#include "LinkQueue.h"
#include "LinkStack.h"
#include "Sort.h"

namespace DTLib
{

template < typename E >
struct Edge : public Object
{
    int b;
    int e;
    E data;

    Edge(int i=-1, int j=-1)
    {
        b = i;
        e = j;
    }

    Edge(int i, int j, const E& value)
    {
        b = i;
        e = j;
        data = value;
    }

    bool operator ==(const Edge<E>& obj)
    {
        return (b == obj.b) && (e == obj.e);
        /*
            如果起點和終點都是相等的, 就認為兩個邊對象相等,
            在 removeVertex 裡面不需要關注權值的大小.
        */
    }

    bool operator !=(const Edge<E>& obj)
    {
        return !(*this == obj);
    }

    bool operator < (const Edge<E>& obj)
    {
        return (data < obj.data);
    }

    bool operator > (const Edge<E>& obj)
    {
        return (data > obj.data);
    }
};

template < typename V, typename E >
class Graph : public Object
{
protected:
    template < typename T >
    DynamicArray<T>* toArray(LinkQueue<T>& queue)
    {
        DynamicArray<T>* ret = new DynamicArray<T>(queue.length());

        if( ret != NULL )
        {
            for(int i=0; i<ret->length(); i++, queue.remove())
            {
                ret->set(i, queue.front());
            }
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create ret object ...");
        }

        return ret;
    }

    SharedPointer< Array< Edge<E> > > getUndirectedEdges()
    {
        DynamicArray< Edge<E> >* ret = NULL;

        if( asUndirected() )
        {
            LinkQueue< Edge<E> > queue;

            for(int i=0; i<vCount(); i++)
            {
                for(int j=i; j<vCount(); j++)
                {
                    if( isAdjacent(i, j) )
                    {
                        queue.add(Edge<E>(i, j, getEdge(i, j)));
                    }
                }
            }

            ret = toArray(queue);
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "This function is for undirected graph only ...");
        }

        return ret;
    }

    int find(Array<int>& p, int v)
    {
        while( p[v] != -1)
        {
            v = p[v];
        }

        return v;
    }
public:
    virtual V getVertex(int i) = 0;
    virtual bool getVertex(int i, V& value) = 0;
    virtual bool setVertex(int i, const V& value) = 0;
    virtual SharedPointer< Array<int> > getAdjacent(int i) = 0;
    virtual bool isAdjacent(int i, int j) = 0;
    virtual E getEdge(int i, int j) = 0;
    virtual bool getEdge(int i, int j, E& value) = 0;
    virtual bool setEdge(int i, int j, const E& value) = 0;
    virtual bool removeEdge(int i, int j) = 0;
    virtual int vCount() = 0;
    virtual int eCount() = 0;
    virtual int OD(int i) = 0;
    virtual int ID(int i) = 0;

    virtual int TD(int i)
    {
        return ID(i) + OD(i);
    }

    bool asUndirected()
    {
        bool ret = true;

        for(int i=0; i<vCount(); i++)
        {
            for(int j=0; j<vCount(); j++)
            {
                if( isAdjacent(i, j) )
                {
                    ret = ret && isAdjacent(j, i) && (getEdge(i, j) == getEdge(j, i));
                }
            }
        }

        return ret;
    }

    SharedPointer< Array< Edge<E> > > prim(const E& LIMIT, const bool MINIUM = true) //参数为理论上的最大权值
    {
        LinkQueue< Edge<E> > ret;

        if( asUndirected() )
        {
            DynamicArray<int> adjVex(vCount());
            DynamicArray<bool> mark(vCount());
            DynamicArray<E> cost(vCount());
            SharedPointer< Array<int> > aj = NULL;
            bool end = false;
            int v = 0; // 習慣性地從頂點0來尋找最小生成樹

            for(int i=0; i<vCount(); i++)
            {
                adjVex[i] = -1;
                mark[i] = false;
                cost[i] = LIMIT;
            }

            mark[v] = true;

            aj = getAdjacent(v);

            for(int j=0; j<aj->length(); j++)
            {
                cost[(*aj)[j]] = getEdge(v, (*aj)[j]);
                adjVex[(*aj)[j]] = v;
            }

            // 最多循環 n 次(頂點數目), 中間發現條件不滿足, 要提前結束.
            for(int i=0; (i<vCount()) && !end; i++)
            {
                E m = LIMIT;
                int k = -1; // 用於紀錄相應最小值的頂點

                for(int j=0; j<vCount(); j++)
                {
                    if( !mark[j] && (MINIUM ? (cost[j] < m) : (cost[j] > m)))
                    {
                        m = cost[j];
                        k = j;
                    }
                }

                end = (k == -1);

                if( !end )
                {
                    ret.add(Edge<E>(adjVex[k], k, getEdge(adjVex[k], k)));

                    mark[k] = true;

                    aj = getAdjacent(k);

                    for(int j=0; j<aj->length(); j++)
                    {
                        if( !mark[(*aj)[j]] && (MINIUM ? (getEdge(k, (*aj)[j]) < cost[(*aj)[j]]) : (getEdge(k, (*aj)[j]) > cost[(*aj)[j]])) )
                        {
                            cost[(*aj)[j]] = getEdge(k, (*aj)[j]);
                            adjVex[(*aj)[j]] = k;
                        }
                    }
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "Prim operator is for undirected graph only...");
        }

        if( ret.length() != (vCount() - 1) )
        {
            THROW_EXCEPTION(InvalidOperationException, "No enough edge for prim operation...");
        }

        return toArray(ret);
    }

    SharedPointer< Array< Edge<E> > > kruskal(const bool MINMUM = true)
    {
        LinkQueue< Edge<E> > ret;

        SharedPointer< Array< Edge<E> > > edges = getUndirectedEdges();

        DynamicArray<int> p(vCount()); //前驱标记数组

        for(int i=0; i<p.length(); i++)
        {
            p[i] = -1;
        }

        Sort::Shell(*edges, MINMUM);

        // (最多循環多少次, 有多少條邊, 就循環多少次) && (如果邊非常多, 但是已經挑選好 n-1 條邊, 循環就該結束)
        for(int i=0; (i<edges->length()) && (ret.length() < (vCount() - 1)); i++)
        {
            int b = find(p, (*edges)[i].b);
            int e = find(p, (*edges)[i].e);

            if( b != e )
            {
                p[e] = b;

                ret.add((*edges)[i]);
            }
        }

        if( ret.length() != (vCount() - 1) )
        {
            THROW_EXCEPTION(InvalidOperationException, "No enough edges for Kruskal operation ...");
        }

        return toArray(ret);
    }

    // 为了保持函數接口的一致性，仿照树里面层次遍历的操作來設計函數原型
    SharedPointer< Array<int> > BFS(int i)
    {
        DynamicArray<int>* ret = NULL;
        /*
            返回的數組裡面存儲的就是圖中頂點的編號

            為什麼要放到數組裡面呢?
            必須得反應一下, 這些頂點被訪問的先後次序,
            通過數組元素的次序來反應頂點的訪問次序.
        */

        if( (0 <= i) && (i < vCount()) )
        {
            LinkQueue<int> q;
            LinkQueue<int> r;
            DynamicArray<bool> visited(vCount());

            for(int j=0; j<visited.length(); j++)
            {
                visited[j] = false;
            }

            q.add(i);

            while( q.length() > 0 )
            {
                int v = q.front();

                q.remove();

                if( !visited[v] )
                {
                    SharedPointer< Array<int> > aj = getAdjacent(v);

                    for(int j=0; j<aj->length(); j++)
                    {
                        q.add((*aj)[j]);
                    }

                    r.add(v);

                    visited[v] = true;
                }
            }

            ret = toArray(r);
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }

        return ret;
    }

    SharedPointer< Array<int> > DFS(int i)
    {
        DynamicArray<int>* ret = NULL;

        if( (0 <= i) && (i < vCount()) )
        {
            LinkStack<int> s;
            LinkQueue<int> r;
            DynamicArray<bool> visited(vCount());

            for(int j=0; j<visited.length(); j++)
            {
                visited[j] = false;
            }

            s.push(i);

            while( s.size() > 0 )
            {
                int v = s.top();

                s.pop();

                if( !visited[v] )
                {
                    SharedPointer< Array<int> > aj = getAdjacent(v);

                    for(int j=aj->length() - 1; j>=0; j--)
                    {
                        s.push((*aj)[j]);
                    }

                    r.add(v);

                    visited[v] = true;
                }
            }

            ret = toArray(r);
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }

        return ret;
    }

    SharedPointer< Array<int> > dijkstra(int i, int j, const E& LIMIT)
    {
        LinkQueue<int> ret;

        if( (0 <= i) && (i < vCount()) && (0 <= j) && (j < vCount()) )
        {
            DynamicArray<E> dist(vCount());
            DynamicArray<int> path(vCount());
            DynamicArray<bool> mark(vCount());

            for(int k=0; k<vCount(); k++)
            {
                mark[k] = false;
                path[k] = -1;

                dist[k] = isAdjacent(i, k) ? (path[k] = i, getEdge(i, k)) : LIMIT;
            }

            mark[i] = true;

            for(int k=0; k<vCount(); k++)
            {
                E m = LIMIT;
                int u = -1;

                for(int w=0; w<vCount(); w++)
                {
                    if( !mark[w] && (dist[w] < m) )
                    {
                        m = dist[w];
                        u = w;
                    }
                }

                // 判斷有沒有最小路徑被找到.
                if( u == -1 )
                {
                    break;
                }
                // 如果圖中只有顶点没有边，找不到两点间最短路徑.

                mark[u] = true;

                for(int w=0; w<vCount(); w++)
                {
                    if( !mark[w] && isAdjacent(u, w) && (dist[u] + getEdge(u, w) < dist[w]) )
                    {
                        dist[w] = dist[u] + getEdge(u, w);
                        path[w] = u;
                    }
                }
            }

            LinkStack<int> s;

            s.push(j);

            // 將從起始頂點 i 到 j 所經歷的各個頂點全部先放到棧裡面去
            for(int k=path[j]; k != -1; k=path[k])
            {
                s.push(k);
            }

            // 將這些頂點從棧裡面彈出來, 放到返回值裡面去
            while( s.size() > 0 )
            {
                ret.add(s.top());

                s.pop();
            }
            /*
                為什麼要先放到棧裡面去?
                因為 path 數組裡面保存的結果, 是由終止頂點不停的推到起始頂點的,
                也就是說, path 數組裡面所保存的頂點順序是逆序的.

                現在要將順序反過來, 靠棧中轉一下.
            */
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index <i, j> is invalid ...");
        }

        if( ret.length() < 2 )
        {
            THROW_EXCEPTION(ArithmeticException, "There is no path from i to j ...");
        }

        return toArray(ret);
    }

    SharedPointer< Array<int> > floyd(int x, int y, const E& LIMIT)
    {
        LinkQueue<int> ret;

        if( (0 <= x) && (x < vCount()) && (0 <= y) && ( y < vCount()) )
        {
            DynamicArray< DynamicArray<E> > dist(vCount());
            DynamicArray< DynamicArray<int> > path(vCount());

            for(int k=0; k<vCount(); k++)
            {
                dist[k].resize(vCount());
                path[k].resize(vCount());
            }

            for(int i = 0; i<vCount(); i++)
            {
                for(int j=0; j<vCount(); j++)
                {
                    path[i][j] = -1;
                    dist[i][j] = isAdjacent(i, j) ? (path[i][j]=j, getEdge(i, j)) : LIMIT;
                    // LIMIT 為理論上面的最大值
                }
            }

            for(int k=0; k<vCount(); k++)
            {
                for(int i = 0; i<vCount(); i++)
                {
                    for(int j=0; j<vCount(); j++)
                    {
                        if( (dist[i][k] + dist[k][j]) < dist[i][j] )
                        {
                            dist[i][j] = dist[i][k] + dist[k][j];
                            path[i][j] = path[i][k];
                        }
                    }
                }
            }

             // 推导到终止顶点为止
            while( (x != -1) && (x != y) )
            {
                ret.add(x);
                 // 最短路径上的各个顶点加到返回值中
                x = path[x][y];
                // 递推的将 path[x][y] 上经过的第一个顶点放入 x 中，然后在下一个递推中从 x 出发再递归处其它顶点.
            }

            // 将最后的一个 x 加入返回值队列中，因为上面 x == y，终止了在返回队列中的加入，所以这里要加入
            if( x != -1 )
            {
                ret.add(x);

            }
        }
        else
        {
            THROW_EXCEPTION(ArithmeticException, "Index <x, y> is invalid ...");
        }

        if( ret.length() < 2 )
        {
            THROW_EXCEPTION(ArithmeticException, "There is no path from x to y ...");
        }

        return toArray(ret);
    }
};

}

#endif // GRAPH_H
