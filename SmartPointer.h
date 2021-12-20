#ifndef SMARTPOINTER_H
#define SMARTPOINTER_H



#include "Pointer.h"
//#define NULL 0

namespace DTLib
{

template<typename T>
class SmartPointer : public Pointer<T>
{
public:
    SmartPointer(T* p = NULL) : Pointer<T>(p)
    {
        // ���캯��ֱ���{�ø�Ę��캯��
    }

    SmartPointer(const SmartPointer<T>& obj)
    {
        this->m_pointer = obj.m_pointer;
        const_cast<SmartPointer<T>&>(obj).m_pointer = NULL;
    }

    SmartPointer<T>& operator = (const SmartPointer<T>& obj)
    {
        if(this != &obj) // ��Ҫ�xֵ�o�Լ�
        {
            /*
              delete m_pointer;

              ����Ȅh�� m_pointer �ɆT׃����ָ��Ķѿ��g,
              ���п�����ɮ����Ē���.
              ���Բ����Ȏ��@������, ��t��Ԓ�xֵ���������d�����Ͳ���
              ������ȫ��.
            */
            T* p = this->m_pointer;
            // ����һ���R�r��׃�� p ���� m_pointer ��ָ��Ķѿ��g

            this->m_pointer = obj.m_pointer;

            const_cast<SmartPointer<T>&>(obj).m_pointer = NULL;

            delete p;
            // �����ځ�ጷ� m_pointer ��ָ��Ķѿ��g

        }

        return *this;
        /*
            �����������֧���B�m���xֵ, ���Ƿ���ָ� m_pointer.
        */
    }

    ~SmartPointer()
    {
        delete this->m_pointer;
    }
    // ���������϶�����Ҫ��, ��t SmartPointer ����ָ�����f�ǂ������
};

}
#endif // SMARTPOINTER_H
