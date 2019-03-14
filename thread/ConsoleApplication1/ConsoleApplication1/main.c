/*
 * @Author: northward
 * @Github: https://github.com/northwardtop
 * @Date: 2019-02-14 19:37:41
 * @LastEditors: northward
 * @LastEditTime: 2019-02-15 21:30:28
 * @Description: 测试pthread_key_create()
 * int pthread_key_create (pthread_key_t *__key, void (*__destr_function) (void *))
 *      创建的线程具有私有数据key
 *      不同线程可以使用同名的数据, 但是具有不同的地址和空间
 *      从TSD池分配, 注销(pthread_key_delete(key))返回到池,
 *      注销不会检查其他线程是否使用中, 并将数据置NULL
 *      线程退出时候调用参数二函数指针
 *      类似于虚函数, 同名但不同类实现方法不同
 * 读写私有数据
 *      int pthread_setspecific
 *      void* pthread_getspecific
 * 删除私有数据
		
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

 //测试私有数据保存结构体
typedef struct _Private_Data
{
	int x;
	int y;
} private_data_t;

/**
 * @description:
 * @param {type}
 * @return:
 */
void main_thread(void *key)
{
	printf("this is my main thread, thread id: %lu, my private data is: %d\n",
		pthread_self(), ((private_data_t *)key)->x);
}

/**
 * @description:
 * @param {type}
 * @return:
 */
void *child_thread1(void *arg)
{
	private_data_t setval;
	setval.x = 55;
	setval.y = 55;

	printf("child thread 1, setting key 55\n");
	printf("before change key: %d, %d\n", \
		((private_data_t *)pthread_getspecific(*(pthread_key_t*)arg))->x, \
		((private_data_t *)pthread_getspecific(*(pthread_key_t*)arg))->y);
	pthread_setspecific(*(pthread_key_t *)arg, &(setval.x)); //设置值为55
	printf("before change key: %d, %d\n", \
		((private_data_t *)pthread_getspecific(*(pthread_key_t*)arg))->x, \
		((private_data_t *)pthread_getspecific(*(pthread_key_t*)arg))->y);
}

/**
 * @description:
 * @param {type}
 * @return:
 */
void *child_thread2(void *arg)
{
	private_data_t setval;
	setval.x = 66;
	setval.y = 66;

	printf("child thread 2, setting key 66\n");
	printf("before change key: %d, %d\n", \
		((private_data_t *)pthread_getspecific(*(pthread_key_t*)arg))->x, \
		((private_data_t *)pthread_getspecific(*(pthread_key_t*)arg))->y);
	pthread_setspecific(*(pthread_key_t *)arg, &(setval.x)); //设置值为66
	printf("before change key: %d, %d\n", \
		((private_data_t *)pthread_getspecific(*(pthread_key_t*)arg))->x, \
		((private_data_t *)pthread_getspecific(*(pthread_key_t*)arg))->y);
}

/**
 * @description:
 * @param {type}
 * @return:
 */
int main(int argc, char const *argv[])
{
	pthread_key_t key; //创建一个私有数据变量
	pthread_t tid1, tid2;
	private_data_t test;
	test.x = 1;
	test.y = 2;
	//创建主线程
	pthread_key_create(&key, main_thread);
	pthread_setspecific(key, &test); //修改key为自定义数据
	//创建两个子线程
	pthread_create(&tid1, NULL, child_thread1, (pthread_key_t*)&key);
	pthread_create(&tid2, NULL, child_thread2, (pthread_key_t*)&key);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_key_delete(key);

	return 0;
}

