//2019/11/24 by xjshi
/*
1)��װ����ԭ�Ӳ�����������
2)����ԭ�Ӳ����Ķ�д��
3����װ���������ļ������������ź�����
	���������������̾�Ⱥ������̵ĸ��ؾ���
4�������׽��ֵ��ļ�������linux��
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
	//������
	class Se_spinlock
	{
	public:
        Se_spinlock(){};
        ~Se_spinlock(){};
	public:
		void ES_spinlock(Es_atomic*lock,int set=1,int spin=2048);
		inline int ES_trylock(Es_atomic*lock,int set=1){ return ( (*lock).get() == 0 && (*lock).cmp_set( 0, set));}
		inline void ES_unlock(Es_atomic*lock,int set=1){ (*lock).set(0);}
	};

	//��д��
	class Se_rwlock
	{
	public:
		void ES_rwlock_wlock(Es_atomic *lock){};
		void ES_rwlock_rlock(Es_atomic *lock){};
		void ES_rwlock_unlock(Es_atomic *lock){};
		void ES_rwlock_downgrade(Es_atomic *lock){};
	};

	//�ļ��׽���fd��(��¼��)
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


	typedef struct shmtx_tt{

		Es_atomic  *lock;
#ifndef WIN32
		Es_atomic  *wait;
		unsigned int     semaphore;
		sem_t          sem;
#endif
		int       fd;
		u_char        *name;
		unsigned int     spin; //
//         struct shmtx_tt()
//         {
//             lock.set(0);
//         }
	} shmtx_t;


	//������
	/*
	linux ��¼������¼���ӽ��̲��ܼ̳У�
	windows �����������̲�sleep������Դ���ڹ����ڴ����棬�������ʹ���ڽ���֮����
	*/
	class Se_shmtx
	{
    public:
		unsigned int Se_shmtx_create(shmtx_t *mtx, void *addr,
			unsigned char *name=(unsigned char*)"shm_lock");
		void Se_shmtx_destroy(shmtx_t *mtx);
		unsigned int Se_shmtx_trylock(shmtx_t *mtx);
		void Se_shmtx_lock(shmtx_t *mtx);
		void Se_shmtx_unlock(shmtx_t *mtx);
		unsigned int Se_shmtx_force_unlock(shmtx_t *mtx, int pid);
	};

}