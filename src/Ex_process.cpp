#include "../include/Ex_process.h"
#include "Ex_socket.h"
namespace libseabase{
    pid_t      Se_process::ngx_pid;
    pid_t      Se_process::ngx_parent;
    socket_t   Se_process::ngx_channel;
    int      Se_process::ngx_process_slot;
    int      Se_process::ngx_last_process = 0;
    ngx_process_t  Se_process::ngx_processes[NGX_MAX_PROCESSES];//进程列表
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
        u_long     on;
        pid_t  pid;
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


        if (respawn != NGX_PROCESS_DETACHED) {

            /* Solaris 9 still has no AF_LOCAL */

            if (socketpair(AF_UNIX, SOCK_STREAM, 0, ngx_processes[s].channel) == -1)
            {
//                 ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
//                     "socketpair() failed while spawning \"%s\"", name);
                return NGX_INVALID_PID;
            }

      //      ngx_log_debug2(NGX_LOG_DEBUG_CORE, cycle->log, 0,
      //          "channel %d:%d",
      //          ngx_processes[s].channel[0],
      //          ngx_processes[s].channel[1]);

            if (ngx_nonblocking(ngx_processes[s].channel[0]) == -1) {
//                 ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
//                     ngx_nonblocking_n " failed while spawning \"%s\"",
//                     name);
                ngx_close_channel(ngx_processes[s].channel/*, cycle->log*/);
                return NGX_INVALID_PID;
            }

            if (ngx_nonblocking(ngx_processes[s].channel[1]) == -1) {
//                 ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
//                     ngx_nonblocking_n " failed while spawning \"%s\"",
//                     name);
                ngx_close_channel(ngx_processes[s].channel/*, cycle->log*/);
                return NGX_INVALID_PID;
            }

            on = 1;
            if (ioctl(ngx_processes[s].channel[0], FIOASYNC, &on) == -1) {
//                 ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
//                     "ioctl(FIOASYNC) failed while spawning \"%s\"", name);
                ngx_close_channel(ngx_processes[s].channel/*, cycle->log*/);
                return NGX_INVALID_PID;
            }

            if (fcntl(ngx_processes[s].channel[0], F_SETOWN, ngx_pid) == -1) {
//                 ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
//                     "fcntl(F_SETOWN) failed while spawning \"%s\"", name);
                ngx_close_channel(ngx_processes[s].channel/*, cycle->log*/);
                return NGX_INVALID_PID;
            }

            if (fcntl(ngx_processes[s].channel[0], F_SETFD, FD_CLOEXEC) == -1) {
//                 ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
//                     "fcntl(FD_CLOEXEC) failed while spawning \"%s\"",
//                     name);
                ngx_close_channel(ngx_processes[s].channel/*, cycle->log*/);
                return NGX_INVALID_PID;
            }

            if (fcntl(ngx_processes[s].channel[1], F_SETFD, FD_CLOEXEC) == -1) {
//                 ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
//                     "fcntl(FD_CLOEXEC) failed while spawning \"%s\"",
//                     name);
                ngx_close_channel(ngx_processes[s].channel/*, cycle->log*/);
                return NGX_INVALID_PID;
            }

            ngx_channel = ngx_processes[s].channel[1];

        } else {
            ngx_processes[s].channel[0] = -1;
            ngx_processes[s].channel[1] = -1;
        }

        ngx_process_slot = s;


        pid = fork();

        switch (pid) {

        case -1:
//             ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
//                 "fork() failed while spawning \"%s\"", name);
            ngx_close_channel(ngx_processes[s].channel/*, cycle->log*/);
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
    void Se_process::master_process_cycle(cycle_t *cycle)
    {
        char              *title;
        u_char            *p;
        size_t             size;
        ngx_int_t          i;
        ngx_uint_t         n, sigio;
        sigset_t           set;
        struct itimerval   itv;
        ngx_uint_t         live;
        ngx_msec_t         delay;
        ngx_listening_t   *ls;
        ngx_core_conf_t   *ccf;

        sigemptyset(&set);
        sigaddset(&set, SIGCHLD);
        sigaddset(&set, SIGALRM);
        sigaddset(&set, SIGIO);
        sigaddset(&set, SIGINT);
        sigaddset(&set, ngx_signal_value(NGX_RECONFIGURE_SIGNAL));
        sigaddset(&set, ngx_signal_value(NGX_REOPEN_SIGNAL));
        sigaddset(&set, ngx_signal_value(NGX_NOACCEPT_SIGNAL));
        sigaddset(&set, ngx_signal_value(NGX_TERMINATE_SIGNAL));
        sigaddset(&set, ngx_signal_value(NGX_SHUTDOWN_SIGNAL));
        sigaddset(&set, ngx_signal_value(NGX_CHANGEBIN_SIGNAL));

        if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                "sigprocmask() failed");
        }

        sigemptyset(&set);


        size = sizeof(master_process);

        for (i = 0; i < ngx_argc; i++) {
            size += ngx_strlen(ngx_argv[i]) + 1;
        }

        title = ngx_pnalloc(cycle->pool, size);
        if (title == NULL) {
            /* fatal */
            exit(2);
        }

        p = ngx_cpymem(title, master_process, sizeof(master_process) - 1);
        for (i = 0; i < ngx_argc; i++) {
            *p++ = ' ';
            p = ngx_cpystrn(p, (u_char *) ngx_argv[i], size);
        }

        ngx_setproctitle(title);


        ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

        ngx_start_worker_processes(cycle, ccf->worker_processes,
            NGX_PROCESS_RESPAWN);
        ngx_start_cache_manager_processes(cycle, 0);

        ngx_new_binary = 0;
        delay = 0;
        sigio = 0;
        live = 1;

        for ( ;; ) {
            if (delay) {
//                 if (ngx_sigalrm) {
//                     sigio = 0;
//                     delay *= 2;
//                     ngx_sigalrm = 0;
//                 }

//                 ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
//                     "termination cycle: %M", delay);
// 
//                 itv.it_interval.tv_sec = 0;
//                 itv.it_interval.tv_usec = 0;
//                 itv.it_value.tv_sec = delay / 1000;
//                 itv.it_value.tv_usec = (delay % 1000 ) * 1000;

//                 if (setitimer(ITIMER_REAL, &itv, NULL) == -1) {
//                     ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
//                         "setitimer() failed");
//                 }
            }

            //ngx_log_debug0(NGX_LOG_DEBUG_EVENT, cycle->log, 0, "sigsuspend");

            sigsuspend(&set);

            ngx_time_update();

            //ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
            //    "wake up, sigio %i", sigio);

            if (ngx_reap) {
                ngx_reap = 0;
                ngx_log_debug0(NGX_LOG_DEBUG_EVENT, cycle->log, 0, "reap children");

                live = ngx_reap_children(cycle);
            }

            //if (!live && (ngx_terminate || ngx_quit)) {
            //    ngx_master_process_exit(cycle);
            //}

            if (ngx_terminate) {
                if (delay == 0) {
                    delay = 50;
                }

                if (sigio) {
                    sigio--;
                    continue;
                }

                sigio = ccf->worker_processes + 2 /* cache processes */;

                if (delay > 1000) {
                    ngx_signal_worker_processes(cycle, SIGKILL);
                } else {
                    ngx_signal_worker_processes(cycle,
                        ngx_signal_value(NGX_TERMINATE_SIGNAL));
                }

                continue;
            }

            if (ngx_quit) {
                ngx_signal_worker_processes(cycle,
                    ngx_signal_value(NGX_SHUTDOWN_SIGNAL));

                ls = cycle->listening.elts;
                for (n = 0; n < cycle->listening.nelts; n++) {
                    if (ngx_close_socket(ls[n].fd) == -1) {
                        ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                            ngx_close_socket_n " %V failed",
                            &ls[n].addr_text);
                    }
                }
                cycle->listening.nelts = 0;

                continue;
            }

            if (ngx_reconfigure) {
                ngx_reconfigure = 0;

                if (ngx_new_binary) {
                    ngx_start_worker_processes(cycle, ccf->worker_processes,
                        NGX_PROCESS_RESPAWN);
                    ngx_start_cache_manager_processes(cycle, 0);
                    ngx_noaccepting = 0;

                    continue;
                }

                ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "reconfiguring");

                cycle = ngx_init_cycle(cycle);
                if (cycle == NULL) {
                    cycle = (ngx_cycle_t *) ngx_cycle;
                    continue;
                }

                ngx_cycle = cycle;
                ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx,
                    ngx_core_module);
                ngx_start_worker_processes(cycle, ccf->worker_processes,
                    NGX_PROCESS_JUST_RESPAWN);
                ngx_start_cache_manager_processes(cycle, 1);

                /* allow new processes to start */
                ngx_msleep(100);

                live = 1;
                ngx_signal_worker_processes(cycle,
                    ngx_signal_value(NGX_SHUTDOWN_SIGNAL));
            }

//             if (ngx_restart) {
//                 ngx_restart = 0;
//                 ngx_start_worker_processes(cycle, ccf->worker_processes,
//                     NGX_PROCESS_RESPAWN);
//                 ngx_start_cache_manager_processes(cycle, 0);
//                 live = 1;
//             }
// 
//             if (ngx_reopen) {
//                 ngx_reopen = 0;
//                 ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "reopening logs");
//                 ngx_reopen_files(cycle, ccf->user);
//                 ngx_signal_worker_processes(cycle,
//                     ngx_signal_value(NGX_REOPEN_SIGNAL));
//             }

//             if (ngx_change_binary) {
//                 ngx_change_binary = 0;
//                 ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "changing binary");
//                 ngx_new_binary = ngx_exec_new_binary(cycle, ngx_argv);
//             }
// 
//             if (ngx_noaccept) {
//                 ngx_noaccept = 0;
//                 ngx_noaccepting = 1;
//                 ngx_signal_worker_processes(cycle,
//                     ngx_signal_value(NGX_SHUTDOWN_SIGNAL));
//             }
        }
    }
    void Se_process::start_worker_processes(cycle_t *cycle, int n, int type)
    {

    }
    void Se_process::worker_process_cycle(cycle_t *cycle, void *data)
    {

    }
    void Se_process::worker_process_init(cycle_t *cycle, int worker)
    {

    }
    void Se_process::worker_process_exit(cycle_t *cycle)
    {

    }
};