#ifndef DTSTRING_H
#define DTSTRING_H

#include "Object.h"
/*
    在為庫實現字符串類的時候, 必須注意要遵守這個庫的規則,
    DTLib 庫的規則是每一個類必須繼承自頂層父類 Object.
*/

namespace DTLib
{

class String : Object
{
protected:
    char* m_str;
    int m_length;

    void init(const char* s);
    bool equal(const char* l, const char* r, int len) const; // 比對函數

    static int* make_pmt(const char* p);
    static int kmp(const char* s, const char* p);
public:
    String();
    String(char c);
    String(const char* s);
    String(const String& s);

    int length() const;
    const char* str() const;
    bool startWith(const char* s) const;
    bool startWith(const String& s) const;
    bool endOf(const char* s) const;
    bool endOf(const String& s) const;
    String& insert(int i, const char* s);
    String& insert(int i, const String& s);
    String& trim();
    int indexOf(const char* s) const;
    int indexOf(const String& s) const;
    String& remove(int i, int len); //删除指定下标處，指定长度的子串
    String& remove(const char* s);
    String& remove(const String& s);
    String& replace(const char* t, const char* s);
    String& replace(const String& t, const char* s);
    String& replace(const char* t, const String& s);
    String& replace(const String& t, const String& s);
    String sub(int i, int len) const;

    char& operator [] (int i);
    char operator [] (int i) const;
    bool operator == (const String& s) const;
    bool operator == (const char* s) const;
    bool operator != (const String& s) const;
    bool operator != (const char* s) const;
    bool operator > (const String& s) const;
    bool operator > (const char* s) const;
    bool operator < (const String& s) const;
    bool operator < (const char* s) const;
    bool operator >= (const String& s) const;
    bool operator >= (const char* s) const;
    bool operator <= (const String& s) const;
    bool operator <= (const char* s) const;

    String operator + (const String& s) const;
    String operator + (const char* s) const;
    String operator += (const String& s); //成员会改变的，所以不能用const修饰了
    String operator += (const char* s);  //成员会改变的，所以不能用const修饰了

    String operator - (const char* s) const;
    String operator - (const String& s) const;
    String& operator -= (const char* s);
    String& operator -= (const String& s);

    String& operator = (const String& s);  //成员会改变的，所以不能用const修饰了
    String& operator = (const char* s);  //成员会改变的，所以不能用const修饰了
    String& operator = (char c);  //成员会改变的，所以不能用const修饰了

    ~String();
};

}


#endif // DTSTRING_H
