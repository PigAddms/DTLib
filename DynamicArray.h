#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include "Array.h"
#include "Exception.h"

namespace DTLib
{

template <typename T>
class DynamicArray : public Array<T>
{
protected:
    int m_length;

    T* copy(T* array, int length, int newlength)  // O(min(len, newlen)) ==> O(n);
    {
        T* ret = new T[newlength];

        if( ret != NULL )
        {
            int size = (length < newlength) ? length : newlength;

            for(int i=0; i<size; i++)
            {
                ret[i] = array[i];
            }
        }

        return ret;
    }

    void update(T* array, int length)  // O(1)
    {
        if( array != NULL )
        {
            T* temp = this->m_array;

            this->m_array = array;
            this->m_length = length;

            delete[] temp;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to update DynamicArray object ...");
        }
    }

    void init(T* array, int length)  // O(1)
    {
        if( array != NULL )
        {
            this->m_array = array;
            this->m_length = length;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to init DynamicArray object ...");
        }
    }

public:
    DynamicArray(int length = 0)    // O(1)
    {
        init(new T[length], length);

        /*
        this->m_array = new T[length];

        if( this->m_array != NULL)
        {
            this->m_length = length;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create DynamicArray object ...");
        }
        */
    }

    DynamicArray(const DynamicArray<T>& obj)        // O(n)
    {
        init(copy(obj.m_array, obj.m_length, obj.m_length), obj.m_length);

        /*
        this->m_array = new T[obj.m_length];

        if(this->m_array != NULL)
        {
            this->m_length = obj.m_length;

            for(int i=0; i<obj.m_length; i++)
            {
                this->m_array[i] = obj.m_array[i];
            }
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create DynamicArray object ...");
        }
        */
    }

    DynamicArray<T>& operator= (const DynamicArray<T>& obj)   // O(n)
    {
        if( this != &obj )
        {
            update(copy(obj.m_array, obj.m_length, obj.m_length), obj.m_length);

            /*
            T* array = new T[obj.m_length];   // 改写到这里
            if( array != NULL )
            {
                for(int i=0; i<obj.m_length; i++)
                {
                    array[i] = obj.m_array[i];
                }

                T* temp = this->m_array;

                this->m_array = array;
                this->m_length = obj.m_length;

                delete[] temp; // 最後在來 delete 的意義是為了異常安全
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to copy object ...");
            }
            */
        }

        return *this;
    }

    int length() const   // O(1)
    {
        return m_length;
    }

    void resize( int length )   // O(n)
    {
        if( length != m_length )
        {
            update(copy(this->m_array, this->m_length, length), length);
/*
            T* array = new T[length];

            if( array != NULL )
            {
                int size = (length < m_length) ? length : m_length;
                // 比較 length 與 m_length, 然後取它們當中較小的值, 因為需要去做元素的複製

                for(int i=0; i<size; i++)
                {
                    array[i] = this->m_array[i];
                }

                T* temp = this->m_array; // 為了異常安全

                this->m_array = array;
                this->m_length = length;

                delete[] temp;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to resize object ...");
            }
*/
        }
    }

    ~DynamicArray()    // O(1)
    {
        delete[] this->m_array;
    }
};

}

#endif // DYNAMICARRAY_H
