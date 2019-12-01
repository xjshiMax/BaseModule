//by xjshi 2019/11/20
/*
1) 简易封装epoll,先不设置基类
2）event 的初始化，add del wait 
*/
#pragma once
#include "basevar.h"
#define NGX_READ_EVENT     (EPOLLIN|EPOLLRDHUP)
#define NGX_WRITE_EVENT    EPOLLOUT

#define NGX_LEVEL_EVENT    0
#define NGX_CLEAR_EVENT    EPOLLET
#define NGX_ONESHOT_EVENT  0x70000000
#define NGX_CLOSE_EVENT    1
typedef struct _event
{

}Se_event;
namespace libseabase
{
	class Ex_epollmodule
	{
	public:
		Ex_epollmodule();
		int Se_epoll_init(cycle_t*cycle,unsigned int timems);
		int Se_epoll_add_event(Se_event*ev,int event,unsigned int flags);
		int Se_epoll_del_event(Se_event*ev,int event,unsigned int flags);

		int Se_process_events(cycle_t*cycle,unsigned timems);
	private:
		int ep;
		struct epoll_event *event_list;
		unsigned int  nevents;
	};
}