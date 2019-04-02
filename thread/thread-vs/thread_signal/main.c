#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>



/*
信号发送接受处理流程: 当前线程内绑定信号处理函数signal(信号, 函数)->
				  其他线程向当前线程发送信号pthread_kill(线程id, 信号)->
				  线程依照绑定的函数进行处理void (*sighandler_t)(int)
信号屏蔽设置流程: 
	   int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset);
    和 sigprocmask 相像
	how:  SIG_BLOCK    将第二个信号集 设置为屏蔽这些信号
		  SIG_UNBLOCK   将第二个信号集中的信号从屏蔽集合中删掉
		  SIG_SETMASK   独占屏蔽，直接将信号屏蔽集设置为第二个信号集
	oldset:  保留旧配置, 以便恢复
	其中SIGKILL  SIGSTOP信号无法操作

信号绑定函数
	 typedef void (*sighandler_t)(int);
     sighandler_t signal(int signum, sighandler_t handler);

线程间发送信号
	 int pthread_kill(pthread_t thread, int sig);
线程掩码设置(屏蔽信号)
	int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset);

*/

//信号处理函数
void handle_usr1(int sig)
{
	printf("handle_usr1: thread %lu recive signal %d\n", pthread_self(), sig);
	return;
}


//没有被禁止任何信号的线程
void* thread_func(void* arg)
{
	printf("thread_func: I am %lu thread, I will receive from main thread signal!\n", pthread_self());
	//SIGUSR1
	signal(SIGUSR1, handle_usr1);
	int* ret = (int*)malloc(sizeof(int));
	*ret = 1111;
	sleep(2);
	pthread_exit(ret);
	return NULL;
}


//被禁止SIGUSR2信号的线程
void* thread_func2(void* arg)
{
	printf("thread_func2: I am %lu thread, I will not receive signal from main thread!\n", pthread_self());
	//SIGUSR1
	signal(SIGUSR2, handle_usr1);
	sigset_t set;
	sigfillset(&set);
	sigdelset(&set, SIGUSR2);
	pthread_sigmask(SIG_SETMASK, &set, NULL);
	int* ret = (int*)malloc(sizeof(int));
	*ret = 2222;
	sleep(3);
	pthread_exit(ret);
	return NULL;
}

/**
实现功能:
	主线程创建两个线程并分别向两个线程发送SIGUSR1和SIGUSR2信号
	线程thread_func不屏蔽任何信号, 绑定handle_usr1处理函数
	线程thread_func2屏蔽SIGUSR2信号, 绑定handle_usr1处理函数
	根据线程是否输出handle_usr1内容判断是否屏蔽信号成功
*/
int main(int argc, const char* argv[])
{
	int* pret = NULL;
	pthread_t tid = -1, tid2=-1;
	if (pthread_create(&tid, NULL, thread_func, NULL) < 0)
		perror("thread create failed!\n");
	if (pthread_create(&tid2, NULL, thread_func2, NULL) < 0)
		perror("thread create failed!\n");
	sleep(5);//等待两个线程绑定信号函数
	printf("%lu,%lu\n", tid, tid2);
	//给线程1发送自定义信号1
	if (pthread_kill(tid, SIGUSR1) != 0)
		perror("send SIGUSR1 failed!\n");
	//给线程2发送信号2
	if (pthread_kill(tid2, SIGUSR2) != 0)
		perror("send SIGUSR2 failed!\n");

	sleep(5);
	pthread_join(tid, (void**)&pret);
	printf("thread1 return %d\n", *pret);
	pthread_join(tid2, (void**)&pret);
	printf("thread1 return %d\n", *pret);
	
	if(pret)
		free(pret);
	return 0;
}


