#include "Exception.h"
#include <cstring>
#include <cstdlib>
#include <iostream>

using namespace std;

namespace DTLib
{

void Exception::init(const char* message, const char* file, int line)
{
    /*
        m_messge = message; --> 這樣子肯定是不行的

        參數 messge 這個指針, 它指向的字符串有可能在棧上, 有可能在堆空間裡面,
        還有可能是全局數據區, 這樣子的話, 我們沒有辦法去控制 message 所指向的外部字符串的生命週期,
        所以說這樣子的寫法是不安全的.

        應該怎麼處理呢?
        我們可以拷貝一份字符串出來.
    */

    m_message = (message ? strdup(message) : NULL); // 將字符串複製一份到堆空間, m_message 就指向堆空間裡面的一個字符串.

    if(file != NULL) // 如果不是空, 證明它指向一個字符串, 這個字符串就是發生異常的文件名.
    {
        char sl[16] = {0}; // 定义一个辅助的字符数组

        itoa(line, sl, 10); // 将发生异常的行号转换成字符串

        // 字符串的拼接
        m_location = static_cast<char*>(malloc(strlen(file) + strlen(sl) + 2));

        if(m_location != NULL)
        {
            m_location = strcpy(m_location, file);
            m_location = strcat(m_location, ":");
            m_location = strcat(m_location, sl);
            // +2: 存储 ':' 与字符串结束符 '/0'
        }
        /*
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "Exception::init");
            // 看似完美, 但是有問題
        }
        */
        /*
              從比較高的角度:
              我們在父類的 init 函數裡面, 拋出了一個子類的對象作為異常,
              老爸都還沒有誕生, 就先將兒子扔出去了, 從邏輯上面說不通, 所以說
              不能這樣子寫.

            從代碼運行的角度:
            運行至 else 分支, 在構建 NoEnoughMemoryException 類的對象時, 會調用
            NoEnoughMemoryException 類的構造函數, 在調用構造函數的時候,
            又會先去調用 Exception 類的構造函數, 在構造函數裡面又會調用 init 函數,
            於是又調回來了, malloc 肯定又會返回空指針, 於是又會構建一個 NoEnoughMemoryException 類的對象,
            又會再次調用構造函數, 最後又調用 init 函數, 這樣變成死循環了.

            最好的做法, else分支直接不要.
        */
    }
    else
    {
        m_location = NULL;
    }
}

Exception::Exception(const char* message)
{
    init(message, NULL, 0);
}
Exception::Exception(const char* file, int line)
{
    init(NULL, file, line);
}
Exception::Exception(const char* message, const char* file, int line)
{
    init(message, file, line);
}


Exception::Exception(const Exception& e)
{
    m_message = strdup(e.m_message);
    m_location = strdup(e.m_location);
    /*
      拷贝构造函数以及赋值操作符要保证, 每一个 Exception 对象,
      它内部的两个成员指针, 所指向的内容都应该是独立的一段堆空间.
    */
}

Exception& Exception:: operator= (const Exception& e)
{
    if(this != &e) // 自赋值就什么都不做
    {
        // 进行深拷贝之前, 先将原来成员指针所指向的堆空间释放掉
        free(m_message);
        free(m_location);

        m_message = strdup(e.m_message);
        m_location = strdup(e.m_location);

    }
    return *this;
}

const char* Exception::message() const
{
    return m_message;
}

const char* Exception::location() const
{
    return m_location;
}

Exception:: ~Exception()
{
    free(m_message);
    free(m_location);
    //cout << "this is ~Exception" << endl;
}

}
