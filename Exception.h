#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "Object.h"

/*
    現在創建的異常類族是給我們的數據結構庫做準備的,
    因此將這些異常類族裡面的類放入我們的名稱空間裡面.
*/
namespace DTLib
{


#define THROW_EXCEPTION(e, m) (throw e(m, __FILE__, __LINE__))
/*   
    第一个参数为异常的类型, 第二个参数为異常的详细讯息

    使用這個宏:
        我們在需要的地方, 直接拋出相應的異常對象就行了, 具体文件名和行号宏自己填上,
        当不能够用异常处理机制的时候，只需用在这里注释掉就可以了

        ==> #define THROW_EXCEPTION(e, m) //(throw e(m, __FILE__, __LINE__))

        意味著這個宏沒有任何的作用, 是一個空的宏.

    宏的意義:
        考慮到古代的C++編譯器, 考慮到庫的可移植性.
*/


class Exception : public Object
{
protected:
    char* m_message;
    char* m_location;

    void init(const char* message, const char* file, int line);
    /*
        輔助函數, 用來初始化, 由於重載了構造函數, 這幾個構造函數內部邏輯是
        差不多的, 因此就可以使用一個輔助函數來完成初始化的工作.
    */
public:
    Exception(const char* message);
    Exception(const char* file, int line);
    Exception(const char* message, const char* file, int line);

    /*
        为什么这个类里面要实现这两个函数呢?
        从 init 函数的编写, 应该就可以看得出, 这个异常类对象
        如果进行初始化, 或者赋值的时后, 应该是深拷贝.
    */
    Exception(const Exception& e);
    Exception& operator= (const Exception& e);

    virtual const char* message() const;
    virtual const char* location() const;

    virtual ~Exception() = 0;
    //virtual ~Exception(); // 做測試暫時將後綴 = 0 去掉.
    /*
        纯虚函数是不需要提供实现的, 纯虚函数的实现等着子类来完成.

        提供纯虚析构函数的函数体是个例外.
        C++规定, 但凡自定义了析构函数, 不管这个析构函数是不是纯虚函数,
        一定要提供实现.

        why?
        由于在析构一个对象的时后, 最后肯定会调用到父类的析构函数.
        试想一下, 如果父类的纯虚析构函数没有具体的实现, 这一连串的析构调用工作,
        最后调用到顶层父类的析构函数时, 如何能完成呢?

    */
};

class ArithmeticException : public Exception
{
public:
    // 構造函數的實現, 直接調用父類的構造函數就可以了.
    ArithmeticException() : Exception(0) { } //NULL = 0
    ArithmeticException(const char* message) : Exception(message) { }
    ArithmeticException(const char* file, int line) : Exception(file, line) { }
    ArithmeticException(const char* message, const char* file, int line) : Exception(message, file, line) { }

    // 實現拷貝構造函數, 調用相應的父類的拷貝構造函數
    ArithmeticException(const ArithmeticException& e) : Exception(e) { }

    // 實現賦值操作符的重載函數, 直接調用父類的賦值操作符重載函數
    ArithmeticException& operator= (const ArithmeticException& e)
    {
        Exception::operator=(e);

        return *this;
    }
};

class IndexOutOfBoundsException : public Exception
{
public:
    IndexOutOfBoundsException() : Exception(0)
    {
    }
    IndexOutOfBoundsException(const char* message) : Exception(message)
    {
    }
    IndexOutOfBoundsException(const char* file, int line) : Exception(file, line)
    {
    }
    IndexOutOfBoundsException(const char* message, const char* file, int line) : Exception(message, file, line)
    {
    }
    IndexOutOfBoundsException(const IndexOutOfBoundsException& e) : Exception(e)
    {
    }
    IndexOutOfBoundsException& operator =(const IndexOutOfBoundsException& e)
    {
        Exception::operator =(e);
        return *this;
    }
};


class NoEnoughMemoryException : public Exception
{
public:
    NoEnoughMemoryException() : Exception(0)
    {
    }
    NoEnoughMemoryException(const char* message) : Exception(message)
    {
    }
    NoEnoughMemoryException(const char *file, int line) : Exception(file, line)
    {
    }
    NoEnoughMemoryException(const char* message, const char* file, int line) : Exception(message, file, line)
    {
    }
    NoEnoughMemoryException(const NoEnoughMemoryException& e) : Exception(e)
    {
    }
    NoEnoughMemoryException& operator =(const NoEnoughMemoryException& e)
    {
        Exception::operator =(e);
        return *this;
    }
};


class InvalidParameterException : public Exception
{
public:
    InvalidParameterException() : Exception(0){}
    InvalidParameterException(const char* message) : Exception(message){}
    InvalidParameterException(const char *file, int line) : Exception(file, line){}
    InvalidParameterException(const char* message, const char* file, int line) : Exception(message, file, line){}
    InvalidParameterException(const InvalidParameterException& e) : Exception(e) {}
    InvalidParameterException& operator =(const InvalidParameterException& e)
    {
        Exception::operator =(e);
        return *this;
    }
};


class NullPointerException : public Exception
{
public:
    NullPointerException() : Exception(0)
    {}
    NullPointerException(const char* message) : Exception(message)
    {}
    NullPointerException(const char *file, int line) : Exception(file, line)
    {}
    NullPointerException(const char* message, const char* file, int line) : Exception(message, file, line)
    {}
    NullPointerException(const NullPointerException& e) : Exception(e)
    {}
    NullPointerException& operator =(const NullPointerException& e)
    {
        Exception::operator =(e);
        return *this;
    }
};


class InvalidOperationException : public Exception
{
public:
    InvalidOperationException() : Exception(0)
    {}
    InvalidOperationException(const char* message) : Exception(message)
    {}
    InvalidOperationException(const char *file, int line) : Exception(file, line)
    {}
    InvalidOperationException(const char* message, const char* file, int line) : Exception(message, file, line)
    {}
    InvalidOperationException(const InvalidOperationException& e) : Exception(e)
    {}
    InvalidOperationException& operator =(const InvalidOperationException& e)
    {
        Exception::operator =(e);
        return *this;
    }
};

}

#endif // EXCEPTION_H

