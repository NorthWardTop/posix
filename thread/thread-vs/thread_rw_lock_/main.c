/*互斥锁,读写锁,条件锁
读写锁(允许读读并发, 不允许读写或写写并发,
	1.初始化销毁pthread_rwlock_t
	int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
	int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,
		const pthread_rwlockattr_t *restrict attr);
	pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
	2.申请(锁定)读锁
	int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);阻塞申请
	int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);不阻塞, 直接返回0或错误
	3.申请写
    int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);非阻塞
    int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);阻塞
	4.解锁(无论读/写)
	int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
		1.解读锁时, 还有其他读锁定, 则系统继续保持读锁定, 但当前解锁线程不再是拥有者,所有的读锁都释放才是解锁状态
		2.解写锁时, 解锁所有写锁定
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <pthreadtypes.h>


#define BUF_SIZE	1024
#define THREAD_NUM	4

void* thread_one(void* arg);
void* thread_two(void* arg);
void* thread_three(void* arg);
void* thread_four(void* arg);

int main(int argc, const char* argv[])
{
	int ret=-1;
	char buf[BUF_SIZE] = "\0";
	pthread_t tid[THREAD_NUM] = { -1 };
	pthread_rwlock_t rwlock;
	//线程返回值
	void* thrd_ret[THREAD_NUM] = { NULL };
	//线程执行函数指针数组
	void* (*thread_func_arr[THREAD_NUM])(void* arg) =
	{
		thread_one,
		thread_two,
		thread_three,
		thread_four
	};
	for (int i = 0; i < THREAD_NUM; ++i)
	{
		ret = pthread_create(&tid[i], NULL, thread_func_arr[i], &rwlock);
		if (ret < 0)
			perror("thread create failed!\n");
	}

	for (int i = 0; i < THREAD_NUM; ++i)
		pthread_join(tid[i], &thrd_ret);


	pthread_rwlock_destroy(&rwlock);
	return 0;
}

void * thread_one(void * arg)
{
	pthread_rwlock_t rwlock = *(pthread_rwlock_t*)arg;


	return NULL;
}

void * thread_two(void * arg)
{
	return NULL;
}

void * thread_three(void * arg)
{
	return NULL;
}

void * thread_four(void * arg)
{
	return NULL;
}
