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

typedef struct ngx_cycle_s {
    void                  ****conf_ctx;
//     ngx_pool_t               *pool;
// 
//     ngx_log_t                *log;
//     ngx_log_t                 new_log;
// 
//     ngx_uint_t                log_use_stderr;  /* unsigned  log_use_stderr:1; */
// 
//     ngx_connection_t        **files;
//     ngx_connection_t         *free_connections;
//     ngx_uint_t                free_connection_n;
// 
//     ngx_module_t            **modules;
//     ngx_uint_t                modules_n;
//     ngx_uint_t                modules_used;    /* unsigned  modules_used:1; */
// 
//     ngx_queue_t               reusable_connections_queue;
//     ngx_uint_t                reusable_connections_n;
// 
//     ngx_array_t               listening;
//     ngx_array_t               paths;
// 
//     ngx_array_t               config_dump;
//     ngx_rbtree_t              config_dump_rbtree;
//     ngx_rbtree_node_t         config_dump_sentinel;
// 
//     ngx_list_t                open_files;
//     ngx_list_t                shared_memory;
// 
//     ngx_uint_t                connection_n;
//     ngx_uint_t                files_n;
// 
//     ngx_connection_t         *connections;
//     ngx_event_t              *read_events;
//     ngx_event_t              *write_events;
// 
//     ngx_cycle_t              *old_cycle;
// 
//     ngx_str_t                 conf_file;
//     ngx_str_t                 conf_param;
//     ngx_str_t                 conf_prefix;
//     ngx_str_t                 prefix;
//     ngx_str_t                 lock_file;
//     ngx_str_t                 hostname;
}cycle_t;


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