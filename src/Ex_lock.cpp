#include "../include/Ex_lock.h"
namespace libseabase
{
	void Se_spinlock::ES_spinlock(Es_atomic*lock,int set,int spin)
	{
		unsigned int i,n;
		for(;;)
		{
			if(lock->get()==0 && lock->cmp_set(0,set))
				return;
			for(n = 1;n < spin;n <<= 1)
			{
				for(i=0;i < n;i++)
				{
					lock->cpu_pause();
				}
				if(lock->get()==0 && lock->cmp_set(0,set))
					return;
			}
			Se_sched_yield();
		}

	}
	int Se_fdlock::Se_trylock_fd(int fd)
	{
#ifdef WIN32
#else
		struct flock fl;
		fl.l_type=F_WRLCK;
		fl.l_whence = SEEK_SET;

		if (fcntl(fd, F_SETLK, &fl) == -1) {
			return errno;
		}
#endif
		return SE_OK;
	}
	int Se_fdlock::Se_lock_fd(int fd)
	{
#ifdef WIN32
#else
		struct flock  fl;

		memset(&fl, 0,sizeof(struct flock));
		fl.l_type = F_WRLCK;
		fl.l_whence = SEEK_SET;

		if (fcntl(fd, F_SETLKW, &fl) == -1) {
			return errno;
		}

#endif
			return SE_OK;
	}
	int Se_fdlock::Se_unlock_fd(int fd)
	{
#ifdef WIN32
#else
		struct flock  fl;

		memset(&fl,0, sizeof(struct flock));
		fl.l_type = F_UNLCK;
		fl.l_whence = SEEK_SET;

		if (fcntl(fd, F_SETLK, &fl) == -1) {
			return  errno;
		}
#endif
		return SE_OK;
	}
//-----------------------------------------------------------------------
	static void static_shmtx_wakeup(shmtx_t *mtx);
	unsigned int Se_shmtx::Se_shmtx_create(shmtx_t *mtx, shmtx_sh_t *addr,
		u_char *name)
	{
		return SE_OK;
	}
	void Se_shmtx::Se_shmtx_destroy(shmtx_t *mtx)
	{

	}
	unsigned int Se_shmtx::Se_shmtx_trylock(shmtx_t *mtx)
	{
		return SE_OK;
	}
	void Se_shmtx::Se_shmtx_lock(shmtx_t *mtx)
	{
		;
	}
	void Se_shmtx::Se_shmtx_unlock(shmtx_t *mtx)
	{

	}
	unsigned int Se_shmtx::Se_shmtx_force_unlock(shmtx_t *mtx, int pid)
	{
		return SE_OK;
	}
};