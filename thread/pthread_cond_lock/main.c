/*
条件变量对象pthread_cond_t
	初始化和销毁
	   int pthread_cond_destroy(pthread_cond_t *cond);
	   int pthread_cond_init(pthread_cond_t *restrict cond,
		   const pthread_condattr_t *restrict attr);
	   pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
	等待条件变量
		int pthread_cond_timedwait(pthread_cond_t *restrict cond,
		pthread_mutex_t *restrict mutex,
		const struct timespec *restrict abstime);指定时间范围内等待
			int pthread_cond_wait(pthread_cond_t *restrict cond,
		pthread_mutex_t *restrict mutex);等待
	通知等待条件变量的线程
	   int pthread_cond_broadcast(pthread_cond_t *cond);通知所有所有线程
	   int pthread_cond_signal(pthread_cond_t *cond);通知第一个线程

*/

/*
本代码将应用实现条件变量和互斥锁实现PV操作

*/


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>

#define BUF_SIZE	10
#define _DEBUG		1

typedef struct _obj_t
{
	int buf[BUF_SIZE];	//缓冲区(循环队列)
	int readpos, writepos;//读指针和写指针
	pthread_mutex_t lock;//互斥锁
	pthread_cond_t noempty;//不空条件
	pthread_cond_t nofull;//满条件
	void(*put)(struct _obj_t* obj, int data);
	int(*get)(struct _obj_t* obj);
	struct _obj_t* _this;//面向对象的方法, 将指向本结构地址
}obj_t;


void obj_init(obj_t* obj);//初始化对象
void put(obj_t* obj, int data);//互斥的放入一个数
int get(obj_t* obj);//互斥的返回并删除一个数
void* producer(void* obj);	//生产者线程
void* consumer(void* obj);	//消费者线程

int main(int argc, const char* argv[])
{
	pthread_t tid1, tid2;
	obj_t obj;
	void* thread_ret1;
	void* thread_ret2;

	pthread_create(&tid1, NULL, producer, &obj);
	pthread_create(&tid2, NULL, consumer, &obj);
	if (tid1 < 0 || tid2 < 0)
		perror("thread create failed!\n");
	pthread_join(tid1, &thread_ret1);
	pthread_join(tid1, &thread_ret2);
	printf("thread return:%d, %d\n", *(int*)thread_ret1, *(int*)thread_ret1);

}

void obj_init(obj_t* obj)//初始化对象
{
	obj->get = get;
	obj->put = put;
	obj->readpos = 0;
	obj->writepos = 0;
	pthread_mutex_init(&obj->lock, NULL);
	pthread_cond_init(&obj->noempty, NULL);
	pthread_cond_init(&obj->nofull, NULL);
	obj->_this = obj;
}

//互斥的放入一个数据(&对象,数据)
void put(obj_t * obj, int data)
{
	pthread_mutex_lock(&obj->lock);//加互斥锁
	//当缓冲区是满的时候进行循环wait
	while ((obj->writepos + 1) % BUF_SIZE == obj->readpos)
	{
		printf("waiting no full signal...\n");
		pthread_cond_wait(&obj->nofull, &obj->lock);//wait之前确保是本线程已经互斥锁定的
	}
	//等待完成后缓冲区已经不满, 加入data
	obj->writepos = (++obj->writepos) % BUF_SIZE;
	obj->buf[obj->writepos] = data;
	//唤醒第一个等待不空的线程, 如果没有线程等待(缓冲区本有数据)则无意义
	pthread_cond_signal(&obj->noempty);
	pthread_mutex_unlock(&obj->lock);//解互斥锁
}

int get(obj_t * obj)
{
	int data = 0;
	pthread_mutex_lock(&obj->lock);
	//当缓冲区是空的, 循环等待不空信号
	while (obj->writepos == obj->readpos)
	{
		printf("waiting no empty signal...\n");
		pthread_cond_wait(&obj->noempty, &obj->lock);
	}
	//当不空后
	data = obj->buf[obj->readpos];
	obj->readpos = (++obj->readpos) % BUF_SIZE;
	pthread_mutex_unlock(&obj->lock);
	return data;
}

void * producer(void * obj)
{
	int t = 0;
	void* ret;
	for (int i = 0; i < 10; ++i)
	{
		t = rand() % 4 + 1;//等待1-5秒
		printf("producer sleep %ds\nput the %d product\n", t, i);
		put(obj, i);
#if _DEBUG
		printf("::%d", i);
#endif
	}
	printf("producer exit!\n");
	ret = errno;
	pthread_exit(ret);
	return NULL;
}

void * consumer(void * obj)
{
	int t = 0;
	void* ret;
	int getval;
	for (int i = 0; i < 10; ++i)
	{
		t = rand() % 4 + 1;//等待1-5秒
		getval = get(obj);
		printf("consumer sleep %ds\nget the %d product\n", t, getval);
	}
	printf("producer exit!\n");
	ret = errno;
	pthread_exit(ret);
	return NULL;
}
