#include "../include/Ex_queue.h"

Se_queue_t *ngx_queue_middle(Se_queue_t*queue)
{
    Se_queue_t *middle,*next;
    middle = SE_queue_head(queue);
    if(middle == SE_queue_last(queue))
        return middle;
    next = SE_queue_head(queue);
    for(;;)
    {
        middle = SE_queue_next(middle);
        next = SE_queue_next(next);
        if(next == SE_queue_last(queue))
            return middle;
        next = SE_queue_next(next);
        if(next == SE_queue_last(queue))
            return middle;
    }
}

void SE_queue_sort(Se_queue_t*queue,int(*cmp)(const Se_queue_t*,const Se_queue_t*))
{
    Se_queue_t *q,*prev,*next;
    q = SE_queue_head(queue);
    if(q == SE_queue_last(queue))
    {
        return;
    }
    for(q = SE_queue_next(q);q != SE_queue_sentinel(queue);q = next)
    {
        prev = SE_queue_prev(q);
        next = SE_queue_next(q);
        do{
            if(cmp(prev,q)<=0)
            {
                break;
            }
            prev = SE_queue_prev(prev);
        }while(prev!=SE_queue_sentinel(queue));
        SE_queue_insert_after(prev,q); //比前面的小就插入
    }
}