//by xjshi 2019/11/20
//���ڶ���̵ļܹ����
/*

*/
//���������ͺ궨�干���ļ�
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#ifdef WIN32
#include <Windows.h>
#else
#include<sys/types.h>
#include <sys/stat.h>
#include<sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include<semaphore.h>
#include <signal.h>
#endif



//����ֵͳһ������
#define		SE_BASEERROR		0
#define		SE_OK				SE_BASEERROR
#define		SE_ERROR			SE_BASEERROR-1

namespace libseabase{

using namespace std;

}