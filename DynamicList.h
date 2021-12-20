#ifndef DYNAMICLIST_H
#define DYNAMICLIST_H

#include "SeqList.h"
#include "Exception.h"

namespace DTLib
{
template <typename T>
class DynamicList : public SeqList<T>
{
protected:
    int m_capacity;   //˳��洢�ռ�Ĵ�С

public:
    DynamicList(int capacity)    //����˳��洢�ռ�
    {
        this->m_array = new T[capacity];
        if( this->m_array != NULL )
        {
            this->m_length = 0;
            this->m_capacity = capacity;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No menory to create DynamicList object ...");
        }
    }

    int capacity() const
    {
        return m_capacity;
    }

    /* ����˳��洢�ռ�Ĵ�С,���ŵ�ǰ�洢�ռ�̫���̫С�ˣ�Ҫ���� */
    void resize(int capacity)
    {
        if( m_capacity != capacity )// ֻ�Ю���Ҫ�����惦���g��С, �ͮ�ǰ�����惦���g��С��ͬ�ĕr��, �Ł������H�Ĺ���.
        {
            T* array = new T[capacity];// �]��ֱ�Ӳ��� this->m_array �Ǟ������������g��С��, ��֤���ݽṹû�ж�ʧ������Ҫ�� m_array ����ָ��ԭ���Ķѿռ�.

            if( array != NULL )
            {
                int length = (this->m_length < capacity ? this->m_length : capacity);

                for( int i=0; i<length; i++ )
                {
                    array[i] = this-> m_array[i];
                    /*
                        �����xֵ�������ܷ����쳣, �������쳣��ʱ��, ������Ա����ֵû�аl���κεĸı�, ��ǰ�����Ա팦����Ȼ�Ϸ����õ�,
                        ֻ���� array ָ��Ķѿռ佫�ᷢ��й©�����Ƕ��� resize �������޷���ȫ��.

                        ��Ϊ array ����洢���� T �ķ�ָ����, �����ָ���͸�ֵ�������п��ܱ�����, ������صČ��F�����Ǖ��l���쳣�ģ�
                        �������ǾͲ��ܹ�ȫ��. ��������﷢�����쳣, ����Ƿ�ָ���� T ���µģ�Ҳ���ǵ��������̎��Ĵ��a�Ć��}, �@�ӵĆ��}�ͽ��ɵ��������̎��Լ����]��.
                    */
                }

                T* temp = this->m_array;
                /*
                    �@�ӌ��Ǟ���������� delete ��ǰ�����惦���g, ��� T �Ƿ�ָ���,
                    ���T��һ������, �K��������������e�撁����һ������, ֱ�� delete[] this -> m_array ,
                    ������Ҫ����������, �������l�xֵ�����Ǜ]���k�����е�, �͟o�����C��ǰ�ľ��Ա�߀�ǺϷ����õ�,
                    ����@�� resize �����́K���Ǯ�����ȫ��, �����l������, �@�����Ա팦��Ͳ���������.
                */
                this->m_array = array;  //��䲻�ᷢ���쳣
                this->m_length = length;    //��䲻�ᷢ���쳣
                this->m_capacity = capacity;    //��䲻�ᷢ���쳣

                delete[] temp;
                /*
                    ���㷢�����쳣��Ȼ���쳣���أ�������Ա������ֵ�Ѿ��Ϸ�����ǰ���Ա����Ҳ�ǺϷ����õ�,
                    �@�Ӿ������ˮ�����ȫ.
                */
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No enough to resize DynamicList object ...");
            }
        }
    }

    ~DynamicList()
    {
        delete[] this->m_array;
    }
};

}

#endif // DYNAMICLIST_H
