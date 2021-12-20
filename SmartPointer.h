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
        // 構造函數直接調用父類的構造函數
    }

    SmartPointer(const SmartPointer<T>& obj)
    {
        this->m_pointer = obj.m_pointer;
        const_cast<SmartPointer<T>&>(obj).m_pointer = NULL;
    }

    SmartPointer<T>& operator = (const SmartPointer<T>& obj)
    {
        if(this != &obj) // 不要賦值給自己
        {
            /*
              delete m_pointer;

              如果先刪除 m_pointer 成員變量所指向的堆空間,
              就有可能造成異常的拋出.
              所以不能先幹這件事情, 否則的話賦值操作符重載函數就不是
              異常安全的.
            */
            T* p = this->m_pointer;
            // 先用一個臨時的變量 p 保存 m_pointer 所指向的堆空間

            this->m_pointer = obj.m_pointer;

            const_cast<SmartPointer<T>&>(obj).m_pointer = NULL;

            delete p;
            // 最後在來釋放 m_pointer 所指向的堆空間

        }

        return *this;
        /*
            返回自身可以支持連續的賦值, 不是返回指針 m_pointer.
        */
    }

    ~SmartPointer()
    {
        delete this->m_pointer;
    }
    // 析構函數肯定是需要的, 否則 SmartPointer 智能指針類依舊是個抽象類
};

}
#endif // SMARTPOINTER_H
