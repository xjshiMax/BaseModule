#pragma once
#include "basevar.h"
namespace libseabase{
typedef int       pid_t;
typedef int       socket_t;
#define NGX_INVALID_PID  -1
#define NGX_MAX_PROCESSES         1024

#define NGX_PROCESS_NORESPAWN     -1
#define NGX_PROCESS_JUST_SPAWN    -2
#define NGX_PROCESS_RESPAWN       -3
#define NGX_PROCESS_JUST_RESPAWN  -4
#define NGX_PROCESS_DETACHED      -5

static unsigned char master_process[]="master process";

typedef void (*spawn_proc_pt) (cycle_t *cycle, void *data);
typedef struct {
    pid_t           pid;
    int                 status;
    socket_t        channel[2];

    spawn_proc_pt   proc;
    void               *data;
    char               *name;

    unsigned            respawn:1;
    unsigned            just_spawn:1;
    unsigned            detached:1;
    unsigned            exiting:1;
    unsigned            exited:1;
} ngx_process_t;
//进程处理函数
    class Se_process
    {
    public:
        static Se_process*getInstance();
        static void release();
        static pid_t ngx_spawn_process(cycle_t *cycle,
            spawn_proc_pt proc, void *data, char *name, int respawn);
        static void master_process_cycle(cycle_t *cycle);
        static void start_worker_processes(cycle_t *cycle, int n, int type);
        static void worker_process_cycle(cycle_t *cycle, void *data);
        static void worker_process_init(cycle_t *cycle, int worker);
        static void worker_process_exit(cycle_t *cycle);
    private:
        Se_process(){}
        ~Se_process(){release();};
        static Se_process*m_instance;
    public:
        static pid_t      ngx_pid;
        static pid_t      ngx_parent;
        static socket_t   ngx_channel;
        static int      ngx_process_slot;
        static int      ngx_last_process;
        static ngx_process_t  ngx_processes[NGX_MAX_PROCESSES];//进程列表
    };
}