//by xjshi 2019/12/9
/*
1)accept锁，进程间同步锁，解决惊群问题
2）accept connect 函数
*/
#pragma once
#include "basevar.h"
#include "Ex_lock.h"
#include "Ex_shmem.h"
namespace libseabase{
class Se_acceptMutex
{
public:
    Se_acceptMutex();
    ~Se_acceptMutex();
    int SE_trylock_accept_mutex(cycle_t*cycle);
    int SE_unlock_accept_mutex(cycle_t*cycle);

    void SE_lock_mutex(cycle_t*cycle);
    unsigned int SE_getAcceptMutexHeld();
private:
    shmtx_t m_shmtx;
    Se_shm_t m_shm;
    unsigned int m_accept_mutex_held;
    unsigned int m_accept_events;
    unsigned int m_accept_disabled;
    unsigned int m_use_accpet_events;
};
}