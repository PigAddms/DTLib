#include <cstring>
#include <cstdlib>
#include "DTString.h"
#include "Exception.h"


using namespace std;

namespace DTLib
{
/* 建立指定字符串的 pmt（部分匹配表）表 */
int* String::make_pmt(const char* p)  //  O(m)，只有一个 for 循环
{
    int len = strlen(p);

    int* ret = static_cast<int*>(malloc(sizeof(int) * len));

    if ( ret != NULL )
    {
        /* 算法第一步 */
        int ll = 0; // 前缀和后缀交集的最大长度数，largest length.

        ret[0] = 0;  // 第0個元素的LL值為0

        /* 算法第二步 */
        for(int i=1; i<len; i++)  // 从第一个下标，也就是第二个字符开始计算
        {
            /* 算法第四步 */
            // 假設不成功時, 再次嘗試擴展
            while( (ll > 0) && (p[ll] != p[i]) ) // 当 ll 值为零时，转到下面 if 继续判断，最后赋值与匹配表，所以顺序不要错.
            {
                ll = ret[ll - 1];  // 从之前匹配的部分匹配值表中，继续和最后扩展的那个字符匹配
            }

            /* 算法的第三步，这是成功的情况 */
            // 比對最後擴展的字符是否相等, 如果相等直接+1.
            if( p[ll] == p[i] )
            {
                ll++;
            }

            ret[i] = ll;   // 部分匹配表里存储部分匹配值 ll
        }
    }

    return ret;
}

/* 在字符串 s 中查找子串 p */
int String::kmp(const char* s, const char* p)  // O(m) + O(n) ==> O(m+n)， 只有一个 for 循环
{
    int ret = -1;
    int sl = strlen(s);
    int pl = strlen(p);
    int* pmt = make_pmt(p);

    if( (pmt != NULL) && (0 < pl) && (pl <= sl) ) // 判断是否要進行查找
    {
        for(int i=0, j=0; i<sl; i++)  // i 的值要小于目标窜长度才可以查找
        {
            // 比对不上的时候, 持续比对
            while( (j > 0) && (s[i] != p[j]) )
            {
                j = pmt[j-1];
                /*
                    配不成功, 查表得到下一個, 應該繼續匹配的位置.

                    如果 j 已經為0了, 就不用再查表了.
                */
            }

            if( s[i] == p[j] )  // 比对第一個字符成功
            {
                j++;   // 比对下一个字符
            }

            if( j == pl )  // 如果 j 最後的值就是子串的长度，就意味着查找到了
            {
                ret = i + 1 - pl;
                /*
                    匹配成功后, i 的值停在最后一个匹配成功的字符上,
                    因此可以利用長度訊息過來求, 究竟從哪個位置開始匹配上的.
                */

                break;
            }
        }
    }

    free(pmt);

    return ret;
}


void String::init(const char* s)
{
    m_str = strdup(s);

    if( m_str )
    {
        m_length = strlen(m_str);
    }
    else
    {
        THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create string object ...");
    }
}

String::String()
{
    init("");
}

String::String(const char* s)
{
    init(s ? s : "");   //空指针就转换成空字符串
}

String::String(const String& s)
{
    init(s.m_str);
}

String::String(char c)
{
    char s[] = {c, '\0'};

    init(s);
}

int String::length() const
{
    return m_length;
}

const char* String::str() const // 字符串对象与传统字符串进行互操作的转换函数
{
    return m_str;
}

bool String::equal(const char* l, const char* r, int len) const
{
    bool ret = true;

    for(int i=0; i<len && ret; i++)
    {
        ret = ret && (l[i] == r[i]);
        // 但凡有一個位置的字符的比對不成功, 當前的 for 循環就直接結束了.

        /*
            上面的 ret 有點多餘的樣子, 下面也可以.
            ret = (l[i] == r[i]);

            可能是因為想要上一個字符比對成功, 才允許比對下一個字符, 因此才這樣寫.
        */
    }

    return ret;
}

bool String::startWith(const char* s) const
{
    bool ret = ( s != NULL );

    if( ret )
    {
        int len = strlen(s);

        ret = (len < m_length) && equal(m_str, s, len);
        /*
            1.  如果參數 s 的長度比當前字符串對象裡面所表示的字符串更長,
                就直接返回 False.

            2.  一個字符一個字符的比對, 只有相應位置的每一個字符比對都成功,
                才能說當前字符串是以 s 開頭的.
        */
    }

    return ret;
}

bool String::startWith(const String& s) const
{
    return startWith(s.m_str);
}

bool String::endOf(const char* s) const
{
    bool ret = ( s != NULL );

    if( ret )
    {
        int len = strlen(s);

        char* str = m_str + (m_length - len);

        ret = (len < m_length) && equal(str, s, len);

    }

    return ret;
}

bool String::endOf(const String& s) const
{
    return endOf(s.m_str);
}

String& String::insert(int i, const char* s) // i 是從0開始數數, 下標 i 前面有 i 個字符.
{
    if( (0 <= i) && (i <= m_length) )
    {
        if( ( s != NULL) && ( s[0] != '\0' ) )
        {
            int len = strlen(s);
            char* str = reinterpret_cast<char*>(malloc(m_length + len + 1));

            if( str != NULL )
            {
                strncpy(str, m_str, i);
                //当前字符串的前 i 个字符拷贝出来到 str.
                strncpy(str + i, s, len);
                // 有幾個字符加多少個數, 因為是有幾個, 所以是數數, 直接相加即可.
                strncpy(str + i + len, m_str + i, m_length - i);

                str[m_length + len] = '\0';

                free(m_str);

                m_str = str;
                m_length = m_length + len;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to insert string value...");
            }
        }
    }
    else
    {
        THROW_EXCEPTION(IndexOutOfBoundsException, "parameter i is invalid...");
    }

    return *this;
}
/*
    返回一個 String 類型的引用是想幹嘛呢?
    就想實現鏈式操作.
*/

String& String::insert(int i, const String& s)
{
    return insert(i, s.m_str);
}

String& String::trim()
{
    int b = 0;
    int e = m_length - 1;

    while( m_str[b] == ' ')b++;
    while( m_str[e] == ' ')e--;

    if( b == 0 ) // 前面是沒有空格的
    {
        m_str[e + 1] = '\0';

        m_length = e + 1;
        /*
            最後一個不為空格字符的位置是 e, 這樣就可以知道
            從 0 到 e 就應該是 trim 過後的當前字符串當中的合法字符,
            從 0 到 e 當中有 e+1 個字符.
        */
    }
    else
    {
        for(int i=0, j=b; j<= e; i++, j++)
        {
            m_str[i] = m_str[j];
        }

        m_str[e - b + 1] = '\0';
        m_length = e - b + 1;
    }

    return *this;
}

int String::indexOf(const char* s) const
{
    return kmp(m_str, s ? s : "");
}

int String::indexOf(const String& s) const
{
    return kmp(m_str, s.m_str);
}

String& String::remove(int i, int len)
{
    if( (0 <= i) && (i < m_length) )
    {
        int n = i;
        int m = i + len;
        // 在 n ~ m 範圍之內的字符通通要刪除掉

        while( (n < m) && (m < m_length) )
        {
            m_str[n++] = m_str[m++];
        }

        m_str[n] = '\0';
        m_length = n;
    }

    return *this;
}

String& String::remove(const char* s)
{
    return remove(indexOf(s), s ? strlen(s) : 0);
}

String& String::remove(const String& s)
{
    return remove(indexOf(s), s.length());
}

String& String::replace(const char* t, const char* s)
{
    int index = indexOf(t); // step 0

    if( index >= 0 )
    {
        remove(t);  // step 1
        insert(index, s); // step 2
    }

    return *this;
}

String& String::replace(const String& t, const char* s)
{
    return replace(t.m_str, s);
    // 不要複製粘貼代碼, 利用已有的函數來實現當前的函數.
}

String& String::replace(const char* t, const String& s)
{
    return replace(t, s.m_str);
}

String& String::replace(const String& t, const String& s)
{
    return replace(t.m_str, s.m_str);
}

String String::sub(int i, int len) const
{
    String ret;

    if( (0 <= i) && (i<m_length) )
    {
        if( len < 0 ) len = 0;
        // 如果要提取的子串長度小于零, 肯定是不可能的事, 要归一化到 0.
        if(len + i > m_length) len = m_length - i;
        /*
            如果要提取的子串長度比當前字符串長度還長, 也是不可能的事情,
            要归一化到 m_length - i.
        */
        char* str = reinterpret_cast<char*>(malloc(len + 1));

        strncpy(str, m_str + i, len);

        str[len] = '\0';

        ret = str;
        // 將抽取出來的子串, 賦值給返回值變量 ret, 就能將子串對象給返回了.
    }
    else
    {
        THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid...");
    }

    return ret;
}

char& String::operator [] (int i)
{
    if( (0 <= i) && (i < m_length) )
    {
        return m_str[i];
    }
    else
    {
        THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ...");
    }
}
/*
    這一個重載的版本是給非 const 對象用的, 所以返回的是引用,
    是引用意味著可以被賦值, 可以出現在賦值符號的左邊.
*/

char String::operator [] (int i) const
{
    return (const_cast<String&>(*this))[i];
}
/*
    對返回值賦值的話:
    错误信息：error: lvalue required as left operand of assignment
    看字面意思是：赋值操作的左操作数必须使用左值。

    如果有生成臨時變量:
    就算對臨時變量賦值, 也跟字符串對象的值沒有關係了, 因為更改的是臨時變量的值.

    如果沒有生成臨時變量:
    直接返回值, 因為是右值, 無法對它賦值.

    --------------

    由於函數是返回值, 不是引用, 因此不會影響 const 對象的值.

*/

bool String::operator == (const String& s) const
{
    return ( strcmp(m_str, s.m_str) == 0 );
}

bool String::operator == (const char* s) const
{
    return ( strcmp(m_str, s ? s : "") == 0 );
}

bool String::operator != (const String& s) const
{
    return !(*this == s);
    // 代碼複用, 利用上面已經實現好的操作符重載函數.
}

bool String::operator != (const char* s) const
{
    return !(*this == s);
}

bool String::operator > (const String& s) const
{
    return (strcmp(m_str, s.m_str) > 0);
}

bool String::operator > (const char* s) const
{
    return (strcmp(m_str, s ? s : "") > 0);
}

bool String::operator < (const String& s) const
{
    return (strcmp(m_str, s.m_str) < 0);
}

bool String::operator < (const char* s) const
{
    return (strcmp(m_str, s ? s : "") < 0);
}

bool String::operator >= (const String& s) const
{
    return (strcmp(m_str, s.m_str) >= 0);
}

bool String::operator >= (const char* s) const
{
    return (strcmp(m_str, s ? s : "") >= 0);
}

bool String::operator <= (const String& s) const
{
    return (strcmp(m_str, s.m_str) <= 0);
}

bool String::operator <= (const char* s) const
{
    return (strcmp(m_str, s ? s : "") <= 0);
}

String String::operator + (const String& s) const
{
    return (*this + s.m_str);
}

String String::operator + (const char* s) const
{
    String ret;

    int len = m_length + strlen(s ? s : "");

    char* str = reinterpret_cast<char*>(malloc(len + 1));

    if( str )
    {
        strcpy(str, m_str);
        strcat(str, s ? s : "");

        free(ret.m_str);

        ret.m_str = str;
        ret.m_length = len;
    }
    else
    {
        THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create str object...");
    }

    return ret;
}
String String::operator += (const String& s)
{
    return (*this = *this + s.m_str); // 直接通過定義來實現
    /*
        在實現裡面用到了賦值操作符, 因此下一步必須實現的就是
        賦值操作的重載.
    */
}

String String::operator += (const char* s)
{
    return (*this = *this + s);
}

String String::operator - (const char* s) const
{
    return String(*this).remove(s);
    /*
        String(*this)
        直接調用構造函數產生一個新的字符串對象,
        新的字符串對象是臨時的, 值與當前字符串對象的值是一模一樣的.

        再接著調用臨時對象的 remove 函數刪除子串,
        最後將刪除結果返回.
    */
}

String String::operator - (const String& s) const
{
    return String(*this).remove(s);
}

String& String::operator -= (const char* s)
{
    return remove(s); // 字符串對象自身會被改變
}

String& String::operator -= (const String& s)
{
    return remove(s); // 字符串對象自身會被改變
}

String& String::operator = (const String& s)
{
    return (*this = s.m_str);
}

String& String::operator = (const char* s)
{
    if( m_str != s )
    {
        char* str = strdup(s ? s: "");

        if( str )
        {
            free(m_str);

            m_str = str;
            m_length = strlen(m_str);
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create str object...");
        }
    }

    return *this;
}

String& String::operator = (char c)
{
    char s[] = {c, '\0'};

    return (*this = s);
}

String::~String()
{
    free(m_str);
}

}
