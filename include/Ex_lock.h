//2019/11/24 by xjshi
/*
1)封装基于原子操作的自旋锁
2)基于原子操作的读写锁
3）封装进程锁（文件锁，自旋，信号量）
	进程锁用来解决多进程惊群，多进程的负载均衡
4）基于套接字的文件锁（仅linux）
*/
#pragma once
#include "atomic_x86.h"
namespace libseabase
{
#ifdef WIN32
#define Se_sched_yield()  Sleep(1)
#else
#define Se_sched_yield()  usleep(1)
#endif
	//自旋锁
	class Se_spinlock
	{
	public:
		Se_spinlock();
		~Se_spinlock();
	public:
		void ES_spinlock(Es_atomic*lock,int set=1,int spin=2048);
		inline void ES_trylock(Es_atomic*lock){  (*lock).get() == 0 && (*lock).cmp_set( 0, 1);}
		inline void ES_unlock(Es_atomic*lock){ (*lock).set(0);}
	};

	//读写锁
	class Se_rwlock
	{
	public:
		void ES_rwlock_wlock(Es_atomic *lock){};
		void ES_rwlock_rlock(Es_atomic *lock){};
		void ES_rwlock_unlock(Es_atomic *lock){};
		void ES_rwlock_downgrade(Es_atomic *lock){};
	};

	//文件套接字fd锁(记录锁)
	class Se_fdlock
	{
	public:
		int Se_trylock_fd(int fd);
		int Se_lock_fd(int fd);
		int Se_unlock_fd(int fd);
	private:

	};

	typedef struct {
		Es_atomic   lock;
#ifndef WIN32
		Es_atomic   wait;
#endif
	} shmtx_sh_t;


	typedef struct {

		Es_atomic  *lock;
#ifndef WIN32
		Es_atomic  *wait;
		unsigned int     semaphore;
		sem_t          sem;
#endif
		int       fd;
		u_char        *name;
		unsigned int     spin;
	} shmtx_t;


	//进程锁
	class Se_shmtx
	{
		unsigned int Se_shmtx_create(shmtx_t *mtx, shmtx_sh_t *addr,
			u_char *name);
		void Se_shmtx_destroy(shmtx_t *mtx);
		unsigned int Se_shmtx_trylock(shmtx_t *mtx);
		void Se_shmtx_lock(shmtx_t *mtx);
		void Se_shmtx_unlock(shmtx_t *mtx);
		unsigned int Se_shmtx_force_unlock(shmtx_t *mtx, int pid);
	};

}