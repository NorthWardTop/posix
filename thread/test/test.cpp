/*
对象:pthread_mutex_t lock
	方式1 int pthread_mutex_init(pthread_mutex_t *restrict mutex,对象
		   const pthread_mutexattr_t *restrict attr);
	方式2 pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	释放:
	pthread_mutex_destroy释放
	申请/修改
	pthread_mutex_lock 阻塞上锁
	pthread_mutex_trylock 非阻塞上锁
	pthread_mutex_unlock 解锁还需上锁人
*/

/**
主线程处理数据输入
从线程处理数据
	互斥操作同一个公共资源
*/
#include <stdio.h>
#include <stdlib.h>
//#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define BUF_SIZE  1024
char buf[BUF_SIZE];

void* handle_thread(void* arg);

int main()
{
	pthread_t tid;
	pthread_mutex_t mutex;
	char c;
	int ret = -1;
	void* thread_ret;
	ret = pthread_create(&tid, NULL, handle_thread, &mutex);
	if (ret < 0)
		perror("create thread failed\n");
	pthread_mutex_lock(&mutex);
	for (int i = 0; (c = getchar()) != 'q' && i < 1024; ++i)
		buf[i] = c;
	pthread_mutex_unlock(&mutex);

	pthread_mutex_lock(&mutex);
	printf("main thread writed public data area:\n");
	puts(buf);
	pthread_mutex_unlock(&mutex);
	pthread_join(tid, &thread_ret);
}

//数据处理线程
//将当前字符ASCII码++
void* handle_thread(void* arg)
{
	int ret = -1;
	pthread_mutex_lock((pthread_mutex_t*)arg);
	int i = 0;
	while (buf[i] != '\0')
		buf[i++]++;
	pthread_mutex_unlock((pthread_mutex_t*)arg);

	pthread_mutex_lock((pthread_mutex_t*)arg);
	printf("child thread writed public data area:\n");
	puts(buf);
	pthread_mutex_unlock((pthread_mutex_t*)arg);

	pthread_exit((void *)&ret);
	return 0;
}