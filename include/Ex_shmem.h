//by xjshi 2019/12/9
/*
1)封装针对于共享内存的调用接口
2）使用mmap进行映射
*/
#pragma once
#include "basevar.h"
namespace libseabase
{

typedef struct {
    unsigned char      *addr;
    size_t       size;
    string    name;
    //ngx_log_t   *log;
    unsigned int   exists;   /* unsigned  exists:1;  */
} Se_shm_t;
class Se_shmem
{
public:
    static int SE_shm_alloc(Se_shm_t *shm);
    static void SE_shm_free(Se_shm_t *shm);
};
}