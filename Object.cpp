#include "Object.h"
#include "cstdlib"
#include <iostream>

using namespace std;

namespace DTLib
{

void* Object::operator new (unsigned int size) throw()
{
    //cout << "Object::operator new: " << size << endl;
    return malloc(size);
}

void Object::operator delete (void* p)
{
    //cout << "Object::operator delete: " << p << endl;
    free(p);
}

void* Object::operator new[] (unsigned int size) throw()
{
    return malloc(size);
}

void Object::operator delete[] (void* p)
{
    free(p);
}

bool Object::operator == (const Object& obj)
{
    return (this == &obj);
}

bool Object::operator != (const Object& obj)
{
    return (this != &obj);
}

Object::~Object()
{
    /*
        雖然說這個析構函數是一個純虛函數,
        但是析構函數的純虛版本也需要提供函數體出來

        析构函数可以是纯虚的，但纯虚析构函数必须有定义体，因为析构函数的调用是在子类中隐含的。
    */
}

}

