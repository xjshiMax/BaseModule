//by xjshi 2019/12/8
#pragma  once
#include "../include/Ex_event_accept.h"
namespace libseabase{

    Se_acceptMutex::Se_acceptMutex()
    {
        Se_shmtx lockshmtx;
        //Se_shm_t shm;
        m_shm.size = sizeof(Es_atomic)*2;
        m_shm.name = "nginx_shared_zone";
        //shm.log = cycle->log;
        Se_shmem::SE_shm_alloc(&m_shm);
        lockshmtx.Se_shmtx_create(&m_shmtx,m_shm.addr);
    }
    Se_acceptMutex::~Se_acceptMutex()
    {
        Se_shmem::SE_shm_free(&m_shm);
        Se_shmtx lockshmtx;
        lockshmtx.Se_shmtx_destroy(&m_shmtx);
    }
int Se_acceptMutex::SE_trylock_accept_mutex(cycle_t*cycle)
{
    Se_shmtx lockshmtx;
    if(lockshmtx.Se_shmtx_trylock(&m_shmtx));
    //lockshmtx.Se_shmtx_lock(&m_shmtx);
    {
        if(m_accept_mutex_held && m_accept_events == 0)
        {
            return SE_OK;
        }
        //if()
        m_accept_events = 0;
        m_accept_mutex_held = 1;
        return SE_OK;
    }
    if(m_accept_mutex_held)
    {
        m_accept_mutex_held = 0;
    }
    return SE_OK;
};
unsigned int Se_acceptMutex::SE_getAcceptMutexHeld()
{
    return m_accept_mutex_held;
}
int Se_acceptMutex::SE_unlock_accept_mutex(cycle_t*cycle)
{
    Se_shmtx lockshmtx;
    lockshmtx.Se_shmtx_unlock(&m_shmtx);
    return SE_OK;
}
void Se_acceptMutex::SE_lock_mutex(cycle_t*cycle)
{
    Se_shmtx lockshmtx;
    lockshmtx.Se_shmtx_lock(&m_shmtx);
}
}