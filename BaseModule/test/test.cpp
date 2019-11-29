// test.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "../../include/Ex_process.h"
#if 0
void show_handler(int sig)
{
    printf("i got signal %d\n",sig);
    int i;
    for(i=0;i<5;i++)
    {
        printf("i=%d\n",i);
        sleep(1);
    }
}
void test1()
{
    int i=0;
    struct sigaction act,oldact;
    act.sa_handler = show_handler;
    sigaddset(&act.sa_mask,SIGQUIT);
    act.sa_flags=SA_RESETHAND | SA_NODEFER;
    sigaction(SIGINT,&act,&oldact);
    while(1)
    {
        sleep(1);
        printf("sleeping %d\n",i);
        i++;
    }}

void test2()
{
    struct sigaction newact,oldact;
    newact.sa_handler = SIG_IGN;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    int count = 0;
    pid_t pid=0;
    sigaction(SIGINT,&newact,&oldact);//��ԭ���ı��ݵ�oldact����
    pid = fork();
    if(pid==0)
     {  while(1)
        {
            printf("i am chhild ....\n");
            sleep(1);
    }}
    while(1)
    {
        if(count++>3)
        {
            sigaction(SIGINT,&oldact,NULL);
            printf("pid=%d\n",pid);
            kill(pid,SIGKILL);//�����̷��źţ�ɱ���ӽ���
        }
        printf("i am father .....\n");
        sleep(1);
    }
}
void handle_fun(int sig)
{
    printf("\n handle_fun -I got signal %d\n",sig);
    //�ָ��ն��ն��ź�SIG_INT��Ĭ����Ϊ
    (void)signal(SIGINT,SIG_DFL);
}
void test3()
{
    //�ı��ն��ж��ź�SIGINTĬ����Ϊ��ʹִ֮��handle_fun����
    signal(SIGINT,handle_fun);
    while(1)
    {
        printf("Hello \n");
        sleep(1);
    }
}
#endif

using namespace libseabase;
int main(int argc, char* argv[])
{
   Se_process*process=Se_process::getInstance();
   cycle_t ct;
   process->master_process_cycle(&ct);
	return 0;
}

