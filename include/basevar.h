//by xjshi 2019/11/20
//基于多进程的架构设计
/*

*/
//基础变量和宏定义共有文件
#pragma once
#include <stdio.h>
#ifdef WIN32
#include <Windows.h>
#else
#include<sys/types.h>
#include<sys/socket.h>
#endif



//返回值统一错误码
#define		SE_BASEERROR		0
#define		SE_OK				SE_BASEERROR
#define		SE_ERROR			SE_BASEERROR-1

namespace libseabase{

using namespace std;

}