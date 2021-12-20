#ifndef CIRCLELIST_H
#define CIRCLELIST_H

#include "LinkList.h"

namespace DTLib
{

template < typename T >
class CircleList : public LinkList<T>
{
protected:
    typedef typename LinkList<T>::Node Node;
    // Node �鸸��e���@�� typename LinkList<T>::Node ��͵Ąe��

    int mod(int i) const    //O(1)
    {
        return (this->m_length == 0) ? 0 : (i % this->m_length);
        // ���ܶ� 0 ��ȡ�N�Ĳ��� + �wһ�� i ��ֵ
    }

    Node* last() const // last ������ȡָ��β�Y�c��ָ��     //O(n)
    {
        return this->position(this->m_length - 1)->next;
        /*
            position ���� i-1 ���ڵ㣬��ȡ next ���Ի�ȡ���һ���ڵ��ָ��.

            �@���Ǐ�朱��˼����ʽ���ҵ�β�Y�c, ֻ���ҵ�һ���Y�c, ���܏��@���Y�c�ҵ���һ���Y�c.
        */

        //return this->position(this->m_length);
        /*
            �@�ӌ���˼����ʽ�Ǐĺ����ĽǶȁ��ҵ�β�Y�c, ��� position(i) ���ص��ǵ� i ���Y�c�ĵ�ַ.
            (�@�e������ʼ�� 1, 2 , 3, ..., �K���Ǐ� 0 �_ʼ����.)
        */

    }

    void last_to_first() const    //��������β����     //O(n)
    {
        last()->next = this->m_header.next;
        // β�Y�c�� next ָ�ָ���׽Y�c
    }

public:
    bool insert(int i, const T& e)  //O(1) + O(n) + O(n) ==> O(2n+1) ==> O(n)
    {
        bool ret = true;

        i = i % (this->m_length + 1); // �wһ�� i ��ֵ       //O(1)

        ret = LinkList<T>::insert(i, e);        //O(n)
        /*
            ʹ�ø�� insert �����팍�F��� insert ����.
            �@Ҳ�Ǟ�ʲ�N i ��Ҫ�wһ����ԭ��, �����ѭ�h朱�, i ��ֵ���ܷǳ���.
        */

        /***** �����ǻ�춆�朱�팍�Fѭ�h朱�, ߀Ҫע����β���B. *****/

        if( ret && (i == 0) )
        {
            last_to_first();    //O(n)
        }
        /*
            ����ڸ�� insert �����e��, �� node->next = current->next; ���,
            ֻҪ�����һ���Y�c�ĕr��, ʹ�� last_to_first() ����,
            �����ٲ���ĽY�c�е� next ָᘶ���ָ���׽Y�c.
        */

        return ret;
    }

    bool insert(const T& e)     //O(n)
    {
        return insert(this->m_length, e); // �{�����d�İ汾
    }

    bool remove(int i)      //O(n)
    {
        bool ret = true;

        i = mod(i);

        if(i == 0) // ����̎��h��λ�Þ� 0 �Ġ�r       //O(n)
        {
            Node* toDel = this->m_header.next;
            // �����h���ĽY�c�����׽Y�c

            if( toDel != NULL )
            {
                this->m_header.next = toDel->next;
                this->m_length--;

                if( this->m_length > 0 ) // ��߀��Ԫ�ص���r��(������Ԫ��), ����Ų��ָ�.
                {
                    last_to_first();    //�ڄh��Ԫ���˞� 0 ����r, ��ĕr�Ĳ��֞� O(n).

                    if( this->m_current == toDel )
                    {
                        this->m_current = toDel->next;
                    }
                }
                else
                {
                    this->m_header.next = NULL;
                    this->m_current = NULL;
                }

                this->destroy(toDel);
                // ����һ�����N���׽Y�c�Ǟ��ˮ�����ȫ
            }
            else
            {
                ret = false;
            }
        }
        else
        {
            ret = LinkList<T>::remove(i);   //O(n)
        }

        return ret;
    }

    bool set(int i, const T& e)     //O(n)
    {
        return LinkList<T>::set(mod(i), e);
    }

    T get(int i) const      //O(n)
    {
        return LinkList<T>::get(mod(i));
    }

    bool get(int i, T& e) const     //O(n)
    {
        return LinkList<T>::get(mod(i), e);
    }

    int find(const T& e) const      //O(n)
    {
        int ret = -1;

        /*
            last()->next = NULL;
            // ѭ�h朱�׃�Ɇ�朱�

            ret = LinkList<T>::find(e);
            // ��ǿ���ֱ���{�ø�� find ����

            last_to_first();
            // �ٻ֏ͳ�ѭ�h朱�

            ���������������, ��� find() �еıȽϲ������Ǳ��^Č���(T value),
            �ǿ϶�Ҫ���d��ȱ��^����������ȱ��^���������ܕ���������.

            ���������ͮ�������, ������û�� try-catch, ��������������,
            last_to_first() �͛]�ЙC�����{����, ������ѭ�h����׃�Ɇ�朱�,
            �@Ȼ�͸�׃ѭ�h朱�Ġ�B, ���ԾͲ��Ǯ�����ȫ��.

            DTLib ���_�l�ĕr���ѽ�����, �ڌ��F���^�̮��в����Sʹ�� try-catch �Z��K,
            �����Ҫ���õ���ֲ��.

            ��������F find ����
        */

        Node* slider = this->m_header.next;

        for(int i=0; i<this->m_length; i++)
        {
            if( slider->value == e )
            {
                ret = i;
                break;
            }

            slider = slider->next;
        }

        return ret;
    }

    void clear()        // O(n+1) ==> O(n)
    {
        /*
            if( this->m_length >0 ) // �����ǰѭ�h朱��e�����нY�c��
            {
                last()->next = NULL;
                LinkList<T>::clear();
            }

            �������@���ӌ��F clear ����.
            clear �����e������l���ˮ���, ���N��ǰ��朱�Ͳ�����ѭ�h朱���.

            ��Ҫ�����F clear ����
        */

        while( this->m_length > 1)      // O(n)
        {
            remove(1);  // ÿ��ѭ�h���Ǆh���˞�1��Ԫ��, �����S�����}��Ҏģ���Ӷ�����, O(1).
            /*
                ������ǰ��ѭ�h朱��e��Y�c�Ă����Ǵ��1,
                ���N�͌��˞� 1 �ĽY�c�o�h����, ֱ����ǰ�ĽY�cֻ��һ��.

                ��ʲ�N�� remove(1) ������ remove(0)��?
                ���Ч��, remove(0) ����ɴ������Ƅ�ָ�Ч�ʵ�, �� remove(1) ���� else ��֧,
                Ч�ʾ��ИO������.
            */
        }

        if( this->m_length == 1)        //O(1)
        {
            Node* toDel = this->m_header.next;
            // ����Ҫ�h�������׽Y�c

            this->m_header.next = NULL;
            this->m_length = 0;
            this->m_current = NULL;

            this->destroy(toDel);
        }
    }

    bool move(int i, int step)      // O(n)
    {
        return LinkList<T>::move(mod(i), step);
    }

    bool end()      // O(1)
    {
        return (this->m_length == 0) || (this->m_current == NULL);
    }
    /*
        myself:
            ������ for ޒȦ�в���ʹ�� !end() ������, ��t��һֱ��v����ѭ�h朱�.
            �@����r�c��朱�ͬ, ��朱�ֻ��vһ��.
    */

    ~CircleList()       // O(n)
    {
        clear();
    }

    /*
      �п����� CircleList ���� LinkList ����, ���� LinkList �еĳ�Ա������Ҫ����麯���ſ���.
    */
};

}


#endif // CIRCLELIST_H
