//2019/11/23 by xjshi
/*
1）创建守护进程
*/
#include "../include/basevar.h"
#include "../Include/Daemon.h"
namespace libseabase
{
#ifndef WIN32
	unsigned int BaseDaemon::SE_Daemon(int &parentPID,int &PID)
	{
		int  fd;

		switch (fork()) {
		case -1:
			//ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "fork() failed");
			return SE_ERROR;
		case 0:
			break;

		default:
			exit(0);
		}

		parentPID = getppid();
		PID = getpid();

		if (setsid() == -1) {
			//ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "setsid() failed");
			return SE_ERROR;
		}

		umask(0);

		fd = open("/dev/null", O_RDWR);
		if (fd == -1) {
// 			ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
// 				"open(\"/dev/null\") failed");
			return SE_ERROR;
		}

		if (dup2(fd, STDIN_FILENO) == -1) {
		//	ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "dup2(STDIN) failed");
			return SE_ERROR;
		}

		if (dup2(fd, STDOUT_FILENO) == -1) {
			//ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "dup2(STDOUT) failed");
			return SE_ERROR;
		}

#if 1
		if (dup2(fd, STDERR_FILENO) == -1) {
	//		ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "dup2(STDERR) failed");
			return SE_ERROR;
		}
#endif

		if (fd > STDERR_FILENO) {
			if (close(fd) == -1) {
			//	ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "close() failed");
				return SE_ERROR;
			}
		}

		return SE_OK;
	}
#else
	unsigned int BaseDaemon::SE_Daemon(int &parentPID,int &PID)
	{
		return SE_OK;
	}
#endif
}