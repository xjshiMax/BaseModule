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
		struct flock fl;
		fl.l_type=F_WRLCK;  
		fl.l_whence = SEEK_SET;

		if (fcntl(fd, F_SETLK, &fl) == -1) {//F_SETLK该标志位设置锁，如果如果失败也会立即放回，不会阻塞进程
			return errno;
		}
		return SE_OK;
	}
	int Se_fdlock::Se_lock_fd(int fd)
	{
		struct flock  fl;

		memset(&fl, 0,sizeof(struct flock));
		fl.l_type = F_WRLCK;
		fl.l_whence = SEEK_SET;

		if (fcntl(fd, F_SETLKW, &fl) == -1) {//F_SETLKW该标志位设置锁，如果如果失败会阻塞进程
			return errno;
		}
		return SE_OK;
	}
	int Se_fdlock::Se_unlock_fd(int fd)
	{
		struct flock  fl;

		memset(&fl,0, sizeof(struct flock));
		fl.l_type = F_UNLCK;
		fl.l_whence = SEEK_SET;

		if (fcntl(fd, F_SETLK, &fl) == -1) {
			return  errno;
		}
	}
//-----------------------------------------------------------------------
	static void static_shmtx_wakeup(shmtx_t *mtx){}
	unsigned int Se_shmtx::Se_shmtx_create(shmtx_t *mtx, shmtx_sh_t *addr,
		u_char *name)
	{
#ifdef WIN32
		mtx->lock = &addr->lock;
		mtx->spin = 2048; //这里默认自旋时间
#else
		if (mtx->name) {

			if (strcmp((const char*)name, (const char*)mtx->name) == 0) {
				mtx->name = name;
				return SE_OK;
			}

			Se_shmtx_destroy(mtx);
		}
		mtx->fd =  open((const char *) name, O_RDWR|O_CREAT|O_BINARY, 0644);
		if(mtx->fd == -1)
		{
			return SE_ERROR;
		}
		unlink((const char *) name); //只需要这个文件在内核中的INODE信息，所以可以删除文件，只需要fd。
		mtx->name = name;
#endif
		return SE_OK;
	}
	void Se_shmtx::Se_shmtx_destroy(shmtx_t *mtx)
	{
#ifdef WIN32

#else
		if(mtx->fd)
			close(mtx->fd);
#endif
	}
	unsigned int Se_shmtx::Se_shmtx_trylock(shmtx_t *mtx)
	{
#ifdef WIN32
		Se_spinlock spinlock;
		int ret = spinlock.ES_trylock(mtx->lock,getpid());
		if(ret!=0)
			return SE_ERROR;
#else
		Se_fdlock fdlock;
		int ret = fdlock.Se_trylock_fd(mtx->fd);
		if(ret!=0)
			return SE_ERROR;
#endif
		return SE_OK;
	}
	void Se_shmtx::Se_shmtx_lock(shmtx_t *mtx)
	{
#ifdef WIN32
		Se_spinlock spinlock;
		spinlock.ES_spinlock(mtx->lock,getpid());
#else
		Se_fdlock fdlock;
		int ret = fdlock.Se_lock_fd(mtx->fd);

#endif
		return ;
	}
	void Se_shmtx::Se_shmtx_unlock(shmtx_t *mtx)
	{
#ifdef WIN32
		Se_spinlock spinlock;
		spinlock.ES_unlock(mtx->lock);
#else
		Se_fdlock fdlock;
		int ret = fdlock.Se_unlock_fd(mtx->fd);
#endif
	}
	unsigned int Se_shmtx::Se_shmtx_force_unlock(shmtx_t *mtx, int pid)
	{
#ifdef WIN32
		Se_spinlock spinlock;
		spinlock.ES_unlock(mtx->lock);
#endif
		return SE_OK;
	}
};