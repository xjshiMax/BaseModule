#pragma once
#include "basevar.h"
#define ngx_nonblocking(s)  fcntl(s, F_SETFL, fcntl(s, F_GETFL) | O_NONBLOCK)
#define ngx_nonblocking_n   "fcntl(O_NONBLOCK)"

#define ngx_blocking(s)     fcntl(s, F_SETFL, fcntl(s, F_GETFL) & ~O_NONBLOCK)
#define ngx_blocking_n      "fcntl(!O_NONBLOCK)"




typedef struct {
    unsigned int  command;
    pid_t   pid;
    int   slot;
    int    fd;
} ngx_channel_t;


// int ngx_write_channel(socket_t s, ngx_channel_t *ch, size_t size,
//     /*ngx_log_t *log*/);
// int ngx_read_channel(socket_t s, ngx_channel_t *ch, size_t size,
//     /*ngx_log_t *log*/);
// int ngx_add_channel_event(cycle_t *cycle, int fd,
//     int event, ngx_event_handler_pt handler);
void ngx_close_channel(int *fd/*ngx_log_t *log*/)
{
    if (close(fd[0]) == -1) {
      //  ngx_log_error(NGX_LOG_ALERT, log, ngx_errno, "close() channel failed");
    }

    if (close(fd[1]) == -1) {
      //  ngx_log_error(NGX_LOG_ALERT, log, ngx_errno, "close() channel failed");
    }
}