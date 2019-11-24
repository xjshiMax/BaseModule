//2019/11/23 by xjshi
/*
1）创建守护进程
*/
#pragma once
namespace libseabase
{
	class BaseDaemon
	{
		//创建守护进程，当主进程调用以后，将变成守护进程
		/*
			parentPID:父进程PID
			PID:	  生成子进程PID
		*/
		static unsigned int SE_Daemon(int &parentPID,int &PID);
	};
}