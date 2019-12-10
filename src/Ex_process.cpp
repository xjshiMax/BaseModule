#include "../include/Ex_process.h"
#include "Ex_socket.h"
#include "Ex_shmem.h"
#define test_ 
namespace libseabase{

#ifdef test_  //test
    int *counter = NULL;
#endif
    pid_t      Se_process::ngx_pid;
    pid_t      Se_process::ngx_parent;
    socket_t   Se_process::ngx_channel;
    int      Se_process::ngx_process_slot;
    int      Se_process::ngx_last_process = 0;
    ngx_process_t  Se_process::ngx_processes[NGX_MAX_PROCESSES];//进程列表
    Se_acceptMutex Se_process::m_accept_mutex;
    Es_atomic*Se_process::m_atomic = new Es_atomic;
    //Se_process::m_atomic->set(0);
//     ngx_signal_t  signals[] = {
//         { ngx_signal_value(NGX_RECONFIGURE_SIGNAL),
//         "SIG" ngx_value(NGX_RECONFIGURE_SIGNAL),
//         "reload",
//         ngx_signal_handler },
// 
//         { ngx_signal_value(NGX_REOPEN_SIGNAL),
//         "SIG" ngx_value(NGX_REOPEN_SIGNAL),
//         "reopen",
//         ngx_signal_handler },
// 
//         { ngx_signal_value(NGX_NOACCEPT_SIGNAL),
//         "SIG" ngx_value(NGX_NOACCEPT_SIGNAL),
//         "",
//         ngx_signal_handler },
// 
//         { ngx_signal_value(NGX_TERMINATE_SIGNAL),
//         "SIG" ngx_value(NGX_TERMINATE_SIGNAL),
//         "stop",
//         ngx_signal_handler },
// 
//         { ngx_signal_value(NGX_SHUTDOWN_SIGNAL),
//         "SIG" ngx_value(NGX_SHUTDOWN_SIGNAL),
//         "quit",
//         ngx_signal_handler },
// 
//         { ngx_signal_value(NGX_CHANGEBIN_SIGNAL),
//         "SIG" ngx_value(NGX_CHANGEBIN_SIGNAL),
//         "",
//         ngx_signal_handler },
// 
//         { SIGALRM, "SIGALRM", "", ngx_signal_handler },
// 
//         { SIGINT, "SIGINT", "", ngx_signal_handler },
// 
//         { SIGIO, "SIGIO", "", ngx_signal_handler },
// 
//         { SIGCHLD, "SIGCHLD", "", ngx_signal_handler },
// 
//         { SIGSYS, "SIGSYS, SIG_IGN", "", NULL },
// 
//         { SIGPIPE, "SIGPIPE, SIG_IGN", "", NULL },
// 
//         { 0, NULL, "", NULL }
//     };
    Se_process*Se_process::m_instance=new Se_process;

    Se_process*Se_process::getInstance()
    {
        return m_instance;
    }
    void Se_process::release()
    {
        if(m_instance)
            delete m_instance;
    }
    pid_t Se_process::ngx_spawn_process(cycle_t *cycle,
        spawn_proc_pt proc, void *data, char *name, int respawn)
    {
        int  s;

        if (respawn >= 0) {
            s = respawn;

        } else {
            for (s = 0; s < ngx_last_process; s++) {
                if (ngx_processes[s].pid == -1) {
                    break;
                }
            }

            if (s == NGX_MAX_PROCESSES) {
//                 ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
//                     "no more than %d processes can be spawned",
//                     NGX_MAX_PROCESSES);
                return NGX_INVALID_PID;
            }
        }

        pid_t pid = fork();

        switch (pid) {

        case -1:
//             ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
//                 "fork() failed while spawning \"%s\"", name);
         //   ngx_close_channel(ngx_processes[s].channel/*, cycle->log*/);
            return NGX_INVALID_PID;

        case 0:
            ngx_parent = ngx_pid;
            ngx_pid = getpid();
            proc(cycle, data);
            break;

        default:
            break;
        }

     //   ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "start %s %P", name, pid);

        ngx_processes[s].pid = pid;
        ngx_processes[s].exited = 0;

        if (respawn >= 0) {
            return pid;
        }

        ngx_processes[s].proc = proc;
        ngx_processes[s].data = data;
        ngx_processes[s].name = name;
        ngx_processes[s].exiting = 0;

        switch (respawn) {

        case NGX_PROCESS_NORESPAWN:
            ngx_processes[s].respawn = 0;
            ngx_processes[s].just_spawn = 0;
            ngx_processes[s].detached = 0;
            break;

        case NGX_PROCESS_JUST_SPAWN:
            ngx_processes[s].respawn = 0;
            ngx_processes[s].just_spawn = 1;
            ngx_processes[s].detached = 0;
            break;

        case NGX_PROCESS_RESPAWN:
            ngx_processes[s].respawn = 1;
            ngx_processes[s].just_spawn = 0;
            ngx_processes[s].detached = 0;
            break;

        case NGX_PROCESS_JUST_RESPAWN:
            ngx_processes[s].respawn = 1;
            ngx_processes[s].just_spawn = 1;
            ngx_processes[s].detached = 0;
            break;

        case NGX_PROCESS_DETACHED:
            ngx_processes[s].respawn = 0;
            ngx_processes[s].just_spawn = 0;
            ngx_processes[s].detached = 1;
            break;
        }

        if (s == ngx_last_process) {
            ngx_last_process++;
        }

        return pid;
    }
void test(int sig)
{
    printf("test get sig:%d\n",sig);
}
    void Se_process::master_process_cycle(cycle_t *cycle)
    {
#ifdef test_ //test
        Se_shm_t shm;
        shm.size = sizeof(int);
        shm.name = "var";
        Se_shmem::SE_shm_alloc(&shm);
        counter = (int*)shm.addr;
        *counter =0;
        Se_shm_t shmspin;
        shmspin.size = sizeof(Es_atomic);
        shmspin.name = "varspin";
        Se_shmem::SE_shm_alloc(&shmspin);
        m_atomic = (Es_atomic*)shmspin.addr;
        m_atomic->set(0);
#endif
        sigset_t           set;
        sigaddset(&set,60);
      //signal(SIGALRM,test);
        struct sigaction act;
        act.sa_handler = test;
        sigaddset(&act.sa_mask,SIGQUIT);
        sigaddset(&act.sa_mask,60);
        act.sa_flags = SA_RESTART;
        sigaction(60,&act,NULL);
        if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {
//             ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
//                 "sigprocmask() failed");
        }

        sigemptyset(&set);
        start_worker_processes(cycle,4,NGX_PROCESS_RESPAWN);
        for(;;)
        {
            sigsuspend(&set);
            printf("after sigsuspend\n");
        }
        //pause();
       
    }
    void Se_process::start_worker_processes(cycle_t *cycle, int n, int type)
    {
//         ngx_int_t      i;
//         ngx_channel_t  ch;
// 
//       //  ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "start worker processes");
// 
//         ngx_memzero(&ch, sizeof(ngx_channel_t));
// 
//         ch.command = NGX_CMD_OPEN_CHANNEL;

        for (int i = 0; i < n; i++) {

            ngx_spawn_process(cycle, worker_process_cycle,
                (void *) (intptr_t) i, "worker process", type);

//             ch.pid = ngx_processes[ngx_process_slot].pid;
//             ch.slot = ngx_process_slot;
//             ch.fd = ngx_processes[ngx_process_slot].channel[0];

          //  ngx_pass_open_channel(cycle, &ch);
    }
    }
    void Se_process::worker_process_cycle(cycle_t *cycle, void *data)
    {
//         sleep(1);
//         sigval  val;
//         sigqueue(getppid(),60,val);
//         pause();
       m_accept_mutex.SE_trylock_accept_mutex(cycle);
       printf("in worker_process_cycle:%d\n",getpid());
       // Se_spinlock spinlock;
       // spinlock.ES_spinlock(m_atomic,getpid());
        for(int j=0;j<100;j++)
        printf("%d\n",(*counter)++);
        m_accept_mutex.SE_unlock_accept_mutex(cycle);
      //  spinlock.ES_unlock(m_atomic,getpid());
        pause();
    }
    void Se_process::worker_process_init(cycle_t *cycle, int worker)
    {

    }
    void Se_process::worker_process_exit(cycle_t *cycle)
    {

    }
    void Se_process::process_events_and_timers(cycle_t *cycle)
    {
        if(m_accept_mutex.SE_trylock_accept_mutex(cycle) == SE_ERROR)
        {
            return;
        }
        if(m_accept_mutex.SE_getAcceptMutexHeld())
        {

        }
  //      m_epollmodule.Se_process_events(cycle,1);
        //SE_process_posted()
        //accept 事件

        if(m_accept_mutex.SE_getAcceptMutexHeld())
        {
            
        }
        m_accept_mutex.SE_unlock_accept_mutex(cycle);
        //超时事件
        //其他事件
    }
};