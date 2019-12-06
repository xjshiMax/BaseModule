//create by xjshi 2019/12/3
/*
1)˫�������������һ��Ԫ��ָ���ͷ����ͷָ���һ��Ԫ�ء�
2����ֲ��nginx-1.14.2
3��c����ʵ��
*/
#pragma once
#include <stddef.h>
typedef struct Se_queue_s Se_queue_t ;
struct Se_queue_s{
    Se_queue_t *prev;
    Se_queue_t *next;
};
//����������h��ʼ�������ǻ��Զ���Ϊ������
#define SE_queue_init(q)                        \
    (q)->prev = q;                              \
    (q)->next = q;

//�����������ʢ�Ƿ�Ϊ�գ����ط�0��Ϊ��
#define SE_queue_empty(h)                       \
    (h) == (h)->prev;
/*
SE_queue_insert_head ��Ԫ��x���뵽��������h��ͷ��
x��Ϊ����Ԫ�ؽṹ����Se_queue_t��ָ��
h����������
*/
#define SE_queue_insert_head(h,x)               \
    (x)->next = (h)->next;                      \
    (x)->next->prev = x;                        \
    (x)->prev = h;                              \
    (h)->next = x;                              \

#define SE_queue_insert_after   SE_queue_insert_head
/*
Se_queue_insert_tail����Ԫ��x��ӵ�����h��β��
h:����ָ��
x:Ϊ����Ԫ�ؽṹ���е�Se_queue_t��Ա��ָ��
*/
#define SE_queue_insert_tail(h,x)               \
    (x)->prev = (h)->prev;                      \
    (x)->prev->next = x;                        \
    (x)->next = h;                              \
    (h)->prev = x;

//��ȡ����h��һ��Ԫ�ؽṹ��ָ��
#define SE_queue_head(h)                        \
    (h)->next

//��ȡ���һ��Ԫ�صĽṹ��ָ��
#define SE_queue_last(h)                        \
    (h)->prev

//��������ṹ���ָ��
#define SE_queue_sentinel(h)                    \
    (h)

//
#define SE_queue_next(q)                        \
    (q)->next

#define SE_queue_prev(q)                        \
    (q)->prev
#define SE_queue_remove(x)                      \
   (x)->next->prev = (x)->prev;                 \
   (x)->prev->next = (x)->next;
  

/*
SE_queue_split ���ڲ������������h��qΪ�磬��ֳ�h,n������
               ����hΪԭ�����ǰ�벿�֣�nΪԭ����ĺ�벿��
h��Ϊ��������
q: ����h�е�һ��Ԫ��
n: ��ֺ�ԭ����ĺ�벿�֣�����q��
*/
#define SE_queue_split(h,q,n)                   \
    (n)->prev = (h)->prev;                      \
    (n)->prev->next = n;                        \
    (n)->next = q;                              \
    (n)->prev = (q)->prev;                      \
    (n)->prev->next = h;                        \
    (q)->prev = n;
   
/*
SE_queue_add:�ϲ�������n������ӵ�h�����β��
h:���������ṹ��Se_queue_t��ָ��
n:��һ������������ָ��
*/
#define SE_queue_add(h,n)                       \
    (h)->prev->next = (n)->next;                \
    (n)->next->prev = (h)->prev;                \
    (h)->prev = (n)->prev;                      \
    (h)->prev->next = h;

/*
���Ի�ȡ��ǰ�ڵ�����ݽṹ���׵�ַ
����struct{
      char a;
      Se_queue_t l;
      int b;
}Testnode;
offsetof �ܷ���link(Se_queue l)�����type(Testnode)�ṹ���ƫ������Ȼ��ͨ����֪�Ľڵ�
    q(Se_queue*)��ȥƫ�����������Ի�ýṹ���ַ��
    Testnode node;
    Testnode*pnode=SE_queue_data(&node.l,Testnode,l)
*/
#define SE_queue_data(q,type,link)              \
    (type*)((unsigned char*)q - offsetof(type,link))

/*
ngx_queue_middle �� ������������Ԫ�أ��������queue��N��Ԫ�أ�����N/2+1��Ԫ��
queue: ����ָ��
*/
Se_queue_t *ngx_queue_middle(Se_queue_t*queue);

/*
SE_queue_sort ��ʹ�ò������򷨶������������cmpfunc��Ҫ�Լ�ʵ��
cmp: �ȽϺ���
*/
void SE_queue_sort(Se_queue_t*queue,int(*cmp)(const Se_queue_t*,const Se_queue_t*));