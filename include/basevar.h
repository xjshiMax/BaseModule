//by xjshi 2019/11/20
//基于多进程的架构设计
/*

*/
//基础变量和宏定义共有文件
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#ifdef WIN32
#include <Windows.h>
#include <process.h>
#else
#include<sys/types.h>
#include <sys/stat.h>
#include<sys/socket.h>
#include <unistd.h>
#include<semaphore.h>
#include <signal.h>
#endif



//返回值统一错误码
#define		SE_BASEERROR		0
#define		SE_OK				SE_BASEERROR
#define		SE_ERROR			SE_BASEERROR-1

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

namespace libseabase{

using namespace std;

}