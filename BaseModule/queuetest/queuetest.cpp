// queuetest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../../include/Ex_queue.h"

typedef struct node_{
    char a;
    Se_queue_t qle;
    int num;
}node;

int _tmain(int argc, _TCHAR* argv[])
{
    node n[5];
    Se_queue_t sequeue;
    SE_queue_init(&sequeue);
    for(int i=0;i<5;i++)
    {
        n[i].a='a'+i;
        n[i].num=i+1;
        SE_queue_insert_tail(&sequeue,&n[i].qle);
    }
    Se_queue_t*q=SE_queue_head(&sequeue);
    do
    {
        //Se_queue_t*q=SE_queue_head(&sequeue);
        node*p=SE_queue_data(q,node,qle);
        printf("%d\n",p->num);
        q=SE_queue_next(q);
    }while(q != SE_queue_sentinel(&sequeue)); 
	return 0;
}

