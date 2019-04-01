#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>

/*
信号发送接受处理流程: 当前线程内绑定信号处理函数signal(信号, 函数)->
				  其他线程向当前线程发送信号pthread_kill(线程id, 信号)->
				  线程依照绑定的函数进行处理void (*sighandler_t)(int)
信号屏蔽设置流程: 


信号绑定函数
	 typedef void (*sighandler_t)(int);
     sighandler_t signal(int signum, sighandler_t handler);

线程间发送信号
	 int pthread_kill(pthread_t thread, int sig);
线程掩码设置(屏蔽信号)
	int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset);

*/


void* thread_func(void* arg)
{
	printf("I am first thread, I will receive from main thread signal!\n");
	SIGUSR1

	int ret = errno;
	pthread_exit(ret);
	return NULL;
}

int main(int argc, const char* argv[])
{
	int* pret = (int*)malloc(sizeof(int));
	pthread_t tid = -1;
	pthread_create(&tid, NULL, thread_func, NULL);


	pthread_join(tid, &pret);
}