//create by xjshi 2019/12/3
/*
1)双向链表，其中最后一个元素指向表头，表头指向第一个元素。
2）移植于nginx-1.14.2
3）c语言实现
*/
#pragma once
#include <stddef.h>
typedef struct Se_queue_s Se_queue_t ;
struct Se_queue_s{
    Se_queue_t *prev;
    Se_queue_t *next;
};
//将链表容器h初始化，这是会自动置为空链表
#define SE_queue_init(q)                        \
    (q)->prev = q;                              \
    (q)->next = q;

//检查链表容器盛是否为空，返回非0即为空
#define SE_queue_empty(h)                       \
    (h) == (h)->prev;
/*
SE_queue_insert_head 将元素x插入到链表容器h的头部
x：为插入元素结构体中Se_queue_t的指针
h：链表容器
*/
#define SE_queue_insert_head(h,x)               \
    (x)->next = (h)->next;                      \
    (x)->next->prev = x;                        \
    (x)->prev = h;                              \
    (h)->next = x;                              \

#define SE_queue_insert_after   SE_queue_insert_head
/*
Se_queue_insert_tail：将元素x添加到链表h的尾端
h:链表指针
x:为插入元素结构体中的Se_queue_t成员的指针
*/
#define SE_queue_insert_tail(h,x)               \
    (x)->prev = (h)->prev;                      \
    (x)->prev->next = x;                        \
    (x)->next = h;                              \
    (h)->prev = x;

//获取链表h第一个元素结构体指针
#define SE_queue_head(h)                        \
    (h)->next

//获取最后一个元素的结构体指针
#define SE_queue_last(h)                        \
    (h)->prev

//返回链表结构体的指针
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
SE_queue_split 用于拆分链表，将链表h以q为界，拆分成h,n两部分
               其中h为原链表的前半部分，n为原链表的后半部分
h：为链表容器
q: 链表h中的一个元素
n: 拆分后原链表的后半部分（包括q）
*/
#define SE_queue_split(h,q,n)                   \
    (n)->prev = (h)->prev;                      \
    (n)->prev->next = n;                        \
    (n)->next = q;                              \
    (n)->prev = (q)->prev;                      \
    (n)->prev->next = h;                        \
    (q)->prev = n;
   
/*
SE_queue_add:合并链表，将n链表添加到h链表的尾端
h:链表容器结构体Se_queue_t的指针
n:另一个链表容器的指针
*/
#define SE_queue_add(h,n)                       \
    (h)->prev->next = (n)->next;                \
    (n)->next->prev = (h)->prev;                \
    (h)->prev = (n)->prev;                      \
    (h)->prev->next = h;

/*
可以获取当前节点的数据结构体首地址
例：struct{
      char a;
      Se_queue_t l;
      int b;
}Testnode;
offsetof 能返回link(Se_queue l)相对于type(Testnode)结构体的偏移量，然后通过已知的节点
    q(Se_queue*)减去偏移量，即可以获得结构体地址。
    Testnode node;
    Testnode*pnode=SE_queue_data(&node.l,Testnode,l)
*/
#define SE_queue_data(q,type,link)              \
    (type*)((unsigned char*)q - offsetof(type,link))

/*
ngx_queue_middle ： 返回链表中心元素，如果链表queue有N个元素，返回N/2+1个元素
queue: 链表指针
*/
Se_queue_t *ngx_queue_middle(Se_queue_t*queue);

/*
SE_queue_sort ：使用插入排序法对链表进行排序，cmpfunc需要自己实现
cmp: 比较函数
*/
void SE_queue_sort(Se_queue_t*queue,int(*cmp)(const Se_queue_t*,const Se_queue_t*));