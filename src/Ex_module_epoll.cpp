#include "../include/Ex_module_epoll.h"
namespace libseabase
{
	Ex_epollmodule::Ex_epollmodule()
	{
		ep=-1;
	}
	int Ex_epollmodule::Se_epoll_init(cycle_t*cycle,unsigned int timems)
	{
		if(ep ==-1)
		{
			ep = epoll_create(1);
			if(ep == -1)
			{
				return SE_ERROR;
			}
		}
	}
	int Ex_epollmodule::Se_epoll_add_event(Se_event*ev,int event,unsigned int flags)
	{
		Se_event;
		unsigned int events;
		events = (unsigned int )event;
		if(event == NGX_READ_EVENT)
		{	
			e=c->write;
			prev = EPOLLOUT;
		}
		else
		{
			e = c->read;
		}
		ee.events = events | (uint32_t) flags;
		ee.data.ptr = (void *) ((uintptr_t) c | ev->instance);
		if (epoll_ctl(ep, op, c->fd, &ee) == -1) {
		
			return SE_ERROR;
		}
	}
	int Ex_epollmodule::Se_epoll_del_event(Se_event*ev,int event,unsigned int flags)
	{
		if(flags & NGX_CLOSE_EVENT)
		{
			ev->active  = 0;
		}
		c = ev->data;

		if (event == NGX_READ_EVENT) {
			e = c->write;
			prev = EPOLLOUT;

		} else {
			e = c->read;
			prev = EPOLLIN|EPOLLRDHUP;
		}

		if (e->active) {
			op = EPOLL_CTL_MOD;
			ee.events = prev | (uint32_t) flags;
			ee.data.ptr = (void *) ((uintptr_t) c | ev->instance);

		} else {
			op = EPOLL_CTL_DEL;
			ee.events = 0;
			ee.data.ptr = NULL;
		}

			c->fd, op, ee.events);

		if (epoll_ctl(ep, op, c->fd, &ee) == -1) {
		
			return SE_ERROR;
		}

		ev->active = 0;

		return NGX_OK;
	}

	int Ex_epollmodule::Se_process_events(cycle_t*cycle,unsigned timems)
	{
		Se_event;
		unsigned int events;
		events
	}
};