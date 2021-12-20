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
        int i = begin;  // ��߅�@·����ʼλ��
        int j = mid + 1;    // ��߅�@·����ʼλ��
        int k = begin;  //�������ռ���ʼλ��

        // i �� j ��߀�]���_��β��
        while( (i <= mid) && (j <= end) )
        {
            // ��·��ǰ�Ĕ���Ԫ���^С, �ͷŵ��o�����g�e��ȥ
            if( min2max ? (src[i] < src[j]) : (src[i] > src[j]) )
            {
                helper[k++] = src[i++];
            }
            else
            {
                helper[k++] = src[j++];
            }
        }

        // �����߅��·߀�]���_��β��, ֱ�ӌ�ʣ�N�Ĕ���Ԫ�ؿ�ؐ���o�����g�e��ȥ.
        while( i <= mid)
        {
            helper[k++] = src[i++];
        }

        // �����߅��·߀�]���_��β��, ֱ�ӌ�ʣ�N�Ĕ���Ԫ�ؿ�ؐ���o�����g�e��ȥ.
        while( j <= end )
        {
            helper[k++] = src[j++];
        }

        // ����K�Y�����o�����gȫ����ؐ��ԭʼ���M�e��ȥ
        for(i = begin; i <= end; i++)
        {
            src[i] = helper[i];
        }
    }

    template < typename T >
    static void Merge(T src[], T helper[], int begin, int end, bool min2max = true)
    {
        /*
            ���ֻ��һ������Ԫ�ص����Ё��f, ����Ҫ�M�ж�·�w��, ��ֱ�Ӿ��������,
            �ڴ��a�e���������� begin == end.

            if( begin == end ) // �ݹ�ĳ��ڣ�ֻ��һ��Ԫ�ص�ʱ�򣬾�������ģ���ʱ begin == end.
            {
                return; // ֱ�ӷ���, ʲ�N������.
            }
            else
            {
                int mid = (begin + end) / 2;

                Merge(src, helper, begin, mid, min2max);
                Merge(src, helper, mid+1, end, min2max);
                Merge(src, helper, begin, mid, end, min2max);
            }
            //��������.
        */

        if( begin < end )
        {
            int mid = (begin + end) / 2;

            Merge(src, helper, begin, mid, min2max);
            Merge(src, helper, mid+1, end, min2max);
            Merge(src, helper, begin, mid, end, min2max); //���������鲢����
        }
        // begin == end �ĕr��ֱ�ӷ���.
    }

    template < typename T >
    static int Partition(T array[], int begin, int end, bool min2max)
    {
        T pv = array[begin]; // ����һ��λ��̎�Ĕ���Ԫ���������.

        while( begin < end ) // �Y���l���@Ȼ�� begin == end �ĕr��.
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
        // ��ͣ��ѭ�h, ֱ�� begin == end, ���ʾ;�λ��.

        array[begin] = pv;  // ��׼��λ

        return begin;
    }

    template < typename T >
    static void Quick(T array[], int begin, int end, bool min2max)
    {
        /*
            �����ʼλ��С춽Kֹλ��, �������������в�ֻһ������Ԫ��.
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
            ������������һ����ӛ, Ĭ�������С��������, ʹ��ʱ���Բ���.

            ��ӛ:
                true ��С��������
                false �Ĵ�С����
        */

        for(int i=0; i<len; i++)
        {
            int min = i; // �o���СԪ���ڔ��M�e�����

            // for ѭ�h�Y����, �ҵ���ǰ����СԪ��.
            for(int j = i + 1; j < len; j++) // ���� i �������Ĕ���Ԫ���Ǯ�ǰ����Сֵ, ��ԓ�� i �����Ԫ���M�бȽ�.
            {
                if( min2max ? (array[min] > array[j]) : (array[min] < array[j]) )
                {
                    min = j;
                }
            }

            if( min != i) // �п��ܮ�ǰ�� i ������Ԫ�ؾ�����Сֵ, �t����Ҫ�M�н��Q.
            {
                Swap(array[i], array[min]);
            }
            /*
                �@�e�� if ���^�ăH�H�ǃɂ�����ֵ�Ƿ����, ��� cpu ���f���Ժܿ�����@�����^,
                ���ǽ��Q�Ͳ�һ����.

                ����f T �������һ�����}�s���, ���N���Q�M�е��}�u���õĕr�g�Ƿǳ����,
                �����������ʱ��, ������ñ��^������ܿ���������, ���N���ñ��^�������_���Q����,
                �@��Ч�ʿ��Ը���.
            */
        }
    }

    template < typename T >     // O(n*n)
    static void Insert(T array[], int len, bool min2max = true)
    {
        for(int i=1; i<len; i++)
        {
            /*
                for ѭ�h��1�_ʼ, ��һ������Ԫ�؛]��Ҫ�M�в���Ĳ���,
                ���Լ��Ϳ��Ԙ���һ�����������.
            */

            int k = i;
            T e = array[i]; // ��������Ĕ���Ԫ���ó���

            // �����^�Ĳ���, ������ǰ�M�б��^, �ھ��w���F�ĕr��, ��һ߅�M�б��^һ߅�M���Ƅ�.
            for(int j=i-1; (j>=0) && (min2max ? (array[j]>e) : (array[j]<e)); j--)
            {
                array[j+1] = array[j];
                k = j;
                /*
                    �Ƅ�֮��, �o�һ��λ��,
                    �������Ƅ��^��ճ����λ�þ��ǔ���Ԫ�� e ��K��λ��.
                */
            }

            /*
                ����ı��^�����������@�δ��a�ă���

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

            if( k != i ) // �������Ƿ��ѽ�������K��λ������?
            {
                array[k] = e;
                // ������Ԫ�طŵ�����K��λ������ȥ
            }
         }
    }

    template < typename T >     //O(n*n)
    static void Bubble(T array[], int len, bool min2max = true)
    {
        bool exchange = true;

        /*
            �����r�ж��ق����ŵĔ���Ԫ�ؾ�Ҫ�M�ж��ٴ�ð�ݵĲ���, ���ǲ�һ����Ҋ�����r,
            �Ƿ�����M���е�ð�ݲ���, ��ه� exchange ���I.
        */
        for(int i=0; (i<len) && exchange; i++)
        {
            exchange = false;

            // �M��ð�ݲ���, �Ӻ���ǰ�M��, ǰ�� i ��λ���Ѿ��ź�����, ���� j>i
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
            d = d / 3 + 1; // d �ļ�С��ʽ
            /*
                �Ĕ��W�ĽǶȁ��C���@���Ӝp�� d ���^���Ƿǳ����y��, ����
                ͨ�^���`�C���@���Ӝp�� d ��Ч�����^�õ�.

                d--;
                Ҳ�����@������, ���ǟo�����Ч��.
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

                if( k != i )   //��ֵ�ȡ��Ƚϲ�����ʱ��
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

        }while( d > 1 ); // Ҫ������ͣ���M�в�������
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
        Array<T>& array ���텢����һ�����M����, ��� Array �Ǹ��,
        ���N���� array ������ļȿ������o�B���M����, Ҳ�����ǄӑB���M����.
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
