#ifndef SORT_H
#define SORT_H

#include "Object.h"
#include "Array.h"

/*
#include <iostream>
using namespace std;
*/

namespace DTLib
{

class Sort : public Object
{
private:
    Sort();
    Sort(const Sort&);
    Sort& operator = (const Sort&);

    template <typename T>
    static void Swap(T& a, T& b)
    {
        T c(a);
        a = b;
        b = c;
    }

    template < typename T >
    static void Merge(T src[], T helper[], int begin, int mid, int end, bool min2max = true)
    {
        int i = begin;  // 左邊這路的起始位置
        int j = mid + 1;    // 右邊這路的起始位置
        int k = begin;  //代表辅助空间起始位置

        // i 和 j 都還沒有達到尾部
        while( (i <= mid) && (j <= end) )
        {
            // 哪路當前的數據元素較小, 就放到輔助空間裡面去
            if( min2max ? (src[i] < src[j]) : (src[i] > src[j]) )
            {
                helper[k++] = src[i++];
            }
            else
            {
                helper[k++] = src[j++];
            }
        }

        // 如果左邊那路還沒有達到尾部, 直接將剩餘的數據元素拷貝到輔助空間裡面去.
        while( i <= mid)
        {
            helper[k++] = src[i++];
        }

        // 如果右邊那路還沒有達到尾部, 直接將剩餘的數據元素拷貝到輔助空間裡面去.
        while( j <= end )
        {
            helper[k++] = src[j++];
        }

        // 將最終結果從輔助空間全部拷貝到原始數組裡面去
        for(i = begin; i <= end; i++)
        {
            src[i] = helper[i];
        }
    }

    template < typename T >
    static void Merge(T src[], T helper[], int begin, int end, bool min2max = true)
    {
        /*
            對於只有一個數據元素的序列來說, 不需要進行二路歸併, 它直接就是有序的,
            在代碼裡面描述就是 begin == end.

            if( begin == end ) // 递归的出口，只有一个元素的时候，就是有序的，此时 begin == end.
            {
                return; // 直接返回, 什麼都不做.
            }
            else
            {
                int mid = (begin + end) / 2;

                Merge(src, helper, begin, mid, min2max);
                Merge(src, helper, mid+1, end, min2max);
                Merge(src, helper, begin, mid, end, min2max);
            }
            //優化如下.
        */

        if( begin < end )
        {
            int mid = (begin + end) / 2;

            Merge(src, helper, begin, mid, min2max);
            Merge(src, helper, mid+1, end, min2max);
            Merge(src, helper, begin, mid, end, min2max); //真正的做归并操作
        }
        // begin == end 的時候直接返回.
    }

    template < typename T >
    static int Partition(T array[], int begin, int end, bool min2max)
    {
        T pv = array[begin]; // 將第一個位置處的數據元素作為基準.

        while( begin < end ) // 結束條件顯然是 begin == end 的時候.
        {
            while( (begin < end) && (min2max ? (array[end] > pv) : (array[end] < pv)) )
            {
                end--;
            }

            Swap(array[begin], array[end]);

            while( (begin < end) && (min2max ? (array[begin] <= pv) : (array[begin] >= pv)) )
            {
                begin++;
            }

            Swap(array[begin], array[end]);
        }
        // 不停的循環, 直到 begin == end, 基準就就位了.

        array[begin] = pv;  // 基准就位

        return begin;
    }

    template < typename T >
    static void Quick(T array[], int begin, int end, bool min2max)
    {
        /*
            如果起始位置小於終止位置, 代表待排序的序列不只一個數據元素.
        */
        if( begin < end )
        {
            int pivot = Partition(array, begin, end, min2max);

            Quick(array, begin, pivot - 1, min2max);
            Quick(array, pivot + 1, end, min2max);
        }
    }


public:
    template < typename T >     // O(n*n)
    static void Select(T array[], int len, bool min2max = true)
    {
        /*
            第三个参数是一個標記, 默认排序从小到大排列, 使用时可以不加.

            標記:
                true 從小到大排序
                false 從大到小排序
        */

        for(int i=0; i<len; i++)
        {
            int min = i; // 紀錄最小元素在數組裡面的下標

            // for 循環結束後, 找到當前的最小元素.
            for(int j = i + 1; j < len; j++) // 假设 i 所對應的數據元素是當前的最小值, 则應該和 i 后面的元素進行比较.
            {
                if( min2max ? (array[min] > array[j]) : (array[min] < array[j]) )
                {
                    min = j;
                }
            }

            if( min != i) // 有可能當前第 i 個數據元素就是最小值, 則不需要進行交換.
            {
                Swap(array[i], array[min]);
            }
            /*
                這裡的 if 比較的僅僅是兩個整數值是否相等, 對於 cpu 來說可以很快完成這個比較,
                但是交換就不一定了.

                如果說 T 的類型是一個很複雜的類, 那麼交換進行的複製所用的時間是非常多的,
                所以在排序的时候, 如果能用比較操作來避开交换操作, 那麼就用比較操作避開交換操作,
                這樣效率可以更好.
            */
        }
    }

    template < typename T >     // O(n*n)
    static void Insert(T array[], int len, bool min2max = true)
    {
        for(int i=1; i<len; i++)
        {
            /*
                for 循環從1開始, 第一個數據元素沒必要進行插入的操作,
                它自己就可以構成一個有序的序列.
            */

            int k = i;
            T e = array[i]; // 將待插入的數據元素拿出來

            // 做比較的操作, 從後向前進行比較, 在具體實現的時候, 是一邊進行比較一邊進行移動.
            for(int j=i-1; (j>=0) && (min2max ? (array[j]>e) : (array[j]<e)); j--)
            {
                array[j+1] = array[j];
                k = j;
                /*
                    移動之後, 紀錄一下位置,
                    因為可能移動過後空出來的位置就是數據元素 e 最終的位置.
                */
            }

            /*
                上面的比較操作是下面這段代碼的優化

                for(int j=i-1; (j>=0); j--)
                {
                    if( array[j] > e)
                    {
                        array[j+1] = array[j];
                        k = j;
                    }
                    else
                    {
                        break;
                    }
            }
            */

            if( k != i ) // 看看它是否已經就在最終的位置上了?
            {
                array[k] = e;
                // 將數據元素放到它最終的位置上面去
            }
         }
    }

    template < typename T >     //O(n*n)
    static void Bubble(T array[], int len, bool min2max = true)
    {
        bool exchange = true;

        /*
            最壞情況有多少個待排的數據元素就要進行多少次冒泡的操作, 但是不一定遇見最壞情況,
            是否會做滿所有的冒泡操作, 依賴於 exchange 標誌.
        */
        for(int i=0; (i<len) && exchange; i++)
        {
            exchange = false;

            // 進行冒泡操作, 从后向前進行, 前面 i 个位置已经排好序了, 所以 j>i
            for(int j=len-1; j>i; j--)
            {
                if(min2max ? (array[j] < array[j-1]) : (array[j] > array[j-1]))
                {
                    Swap(array[j], array[j-1]);
                    exchange = true;
                }
            }
        }
    }

    template < typename T >
    static void Shell(T array[], int len, bool min2max = true)
    {
        int d = len;
        do
        {
            d = d / 3 + 1; // d 的减小方式
            /*
                從數學的角度來證明這樣子減少 d 比較好是非常困難的, 但是
                通過實踐證明這樣子減少 d 是效果比較好的.

                d--;
                也可以這樣子做, 但是無法提高效率.
            */

            for(int i=d; i<len; i+=d)
            {
                int k = i;
                T e = array[i];

                for(int j=i-d; (j>=0) && (min2max ? (array[j] > e) : (array[j] < e)); j-=d)
                {
                    array[j+d] = array[j];
                    k = j;
                }

                if( k != i )   //赋值比“比较操作耗时”
                {
                    array[k] = e;
                }
            }

            /*
            for(int i=0; i < len; i++)
            {
                cout << array[i] << " ";
            }
            cout << endl;
            */

        }while( d > 1 ); // 要反覆不停的進行插入排序
    }

    template < typename T >
    static void Merge(T array[], int len, bool min2max = true)
    {
        T* helper = new T[len];

        if( helper != NULL )
        {
            Merge(array, helper, 0, len-1, min2max);
        }

        delete[] helper;
    }

    template < typename T >
    static void Quick(T array[], int len, bool min2max = true)
    {
        Quick(array, 0, len - 1, min2max);
    }

    /*
        Array<T>& array 代表參數是一個數組對象, 由於 Array 是父類,
        那麼參數 array 所代表的既可以是靜態數組對象, 也可以是動態數組對象.
    */
    template < typename T >
    static void Select(Array<T>& array, bool min2max = true)
    {
        Select(array.array(), array.length(), min2max);
    }

    template < typename T >
    static void Insert(Array<T>& array, bool min2max = true)
    {
        Insert(array.array(), array.length(), min2max);
    }

    template < typename T >
    static void Bubbble(Array<T>& array, bool min2max = true)
    {
        Bubble(array.array(), array.length(), min2max);
    }

    template < typename T >
    static void Shell(Array<T>& array, bool min2max = true)
    {
        Shell(array.array(), array.length(), min2max);
    }

    template < typename T >
    static void Merge(Array<T>& array, bool min2max = true)
    {
        Merge(array.array(), array.length(), min2max);
    }

    template < typename T >
    static void Quick(Array<T>& array, bool min2max = true)
    {
        Quick(array.array(), array.length(), min2max);
    }
};

}

#endif // SORT_H
