//2019/11/23 by xjshi
/*
1�������ػ�����
*/
#pragma once
namespace libseabase
{
	class BaseDaemon
	{
		//�����ػ����̣��������̵����Ժ󣬽�����ػ�����
		/*
			parentPID:������PID
			PID:	  �����ӽ���PID
		*/
		static unsigned int SE_Daemon(int &parentPID,int &PID);
	};
}