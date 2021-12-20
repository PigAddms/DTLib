#ifndef LIST_H
#define LIST_H

#include "Object.h"

namespace DTLib
{

// 因為是用模板的方式來描述線性表, 所以 List.cpp 可以不要了
template < typename T>
class List : public Object
{
protected:
    List(const List&);
    List& operator= (const List &);
    /*
        手工添加構造函數, 編譯器就不會為我們提供默認的構造函數,
        需要自己手工添加一個 public 構造函數.
    */
public:
    List(){}
    virtual bool insert(const T& e) = 0; // 直接往線性表的尾部添加一個新的元素
    virtual bool insert(int i, const T& e) = 0;
    virtual bool remove(int i) = 0;
    virtual bool set(int i, const T& e) = 0;
    virtual bool get(int i, T& e) const = 0;
    virtual int find(const T& e) const = 0;
    virtual int length() const = 0;
    virtual void clear() = 0;
};

}

#endif // LIST_H
