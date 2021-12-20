#ifndef SHAREDPOINTER_H
#define SHAREDPOINTER_H

#include "Pointer.h"
#include <cstdlib>
#include "Exception.h"

namespace DTLib
{

template <typename T>
class SharedPointer : public Pointer<T>
{
protected:
    int* m_ref;

    void assign(const SharedPointer<T>& obj)
    {
        this->m_ref = obj.m_ref;
        this->m_pointer = obj.m_pointer;

        if( this->m_ref )  // obj Ҳ����Ϊ��
        {
            (*this->m_ref)++;
        }
    }

public:
    SharedPointer(T* p = NULL) : m_ref(NULL)
    {
        if( p )
        {
            this->m_ref = static_cast<int*>(std::malloc(sizeof(int)));

            if( this->m_ref )
            {
                *(this->m_ref) = 1;
                this->m_pointer = p;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to creat SharedPointer object ...");
            }
        }
    }

    SharedPointer(const SharedPointer<T>& obj) : Pointer<T>(NULL)  // ��ʼ����ֵ���ã�����Ϊ����������������ʾ���ø��๹�캯��
    {
        assign(obj);
    }

    SharedPointer<T>& operator= (const SharedPointer<T>& obj)
    {
        if( this != &obj) // �������xֵ
        {
            clear();
            /*
                Ҳ�S��ǰ�� SharedPointer �����ѽ�ָ����һƬ�ѿ��g,
                ���Ը�ֵǰ��ԓ���ÿգ����䲻��ָ���κεĶѿ��g.
            */
            assign(obj);
        }

        return *this;
    }

    void clear()
    {
        T* toDel = this->m_pointer;
        int* ref = this->m_ref;

        this->m_ref = NULL;
        this->m_pointer = NULL;

        /*
            �F����һ������ָᘌ�����ָ��һƬ�ѿ��g,
            �@Ƭ�ѿ��g���P����Ӌ��׃����ԓ-1
        */
        if( ref )
        {
            (*ref)--;

            if( *ref == 0 )
            {
                free(ref);

                delete toDel;
            }
        }
    }

    ~SharedPointer()
    {
        clear();
    }
};

/* ͨ��ȫ�ֺ����ķ�ʽ��������ȱ��^������ */
template <typename T>
bool operator== (const SharedPointer<T>& l, const SharedPointer<T>& r)
{
    return (l.get() == r.get());  // ��ָ��Ķѿռ���һ�������
    /*
        get �����K����һ�� const �ɆT����, ���Բ��ܱ� const �������{��.

        ���N����?
        => ���������� const �ɆT����
    */
}

/* ͨ��ȫ�ֺ����ķ�ʽ�����ز��ȱ��^������ */
template <typename T>
bool operator!= (const SharedPointer<T>& l, const SharedPointer<T>& r)
{
    return !(l == r);
    // ����ȱ��^������������ʹ����ȱ��^�����������d��ʵ��
}

}

#endif // SHAREDPOINTER_H
