/*
线程属性解决的问题:设置线程能够申请到的资源(栈空间信息)权限
typedef struct
	{
		   int                           detachstate;     线程的分离状态 既是否可以被等待pthread_join
														PTHREAD_CREATE_JOINABLE   able
														PTHREAD_CREATE_JOINUNABLE unable

		   int                          schedpolicy;   线程调度策略  设置当前线程的调度策略
														基于FIFO,先创建先运行
														时间片
														优先级

		   struct sched_param      schedparam;   线程的调度参数  向调度当前(上个参数)策略传输参数

		   int                          inheritsched;    线程的继承性 是否继承创建线程的调度策略

		   int                          scope;          线程的作用域
		   size_t                      guardsize; 线程栈末尾的警戒缓冲区大小
												  默认值 PAGESIZE

		   int                         stackaddr_set;  设置线程栈的基地址
		   void *                     stackaddr;      线程栈的位置
		   size_t                      stacksize;       线程栈的大小
	}pthread_attr_t;

	pthread_attr_t  attr

	Linux系统对pthread_attr_t的设置是不透明,不能attr.xxx设置,只能API设置

   只能使用特定的系统提供的API来进行相关设置
   初始化线程属性
   设置/获取 detachstate
   设置/获取 线程栈的属性
		1.如何设置栈的大小属性
		2.如果设置stack的地址
		3.如何设置栈的保护区

	线程ID的属性
		pthread_t pthread_self(void)

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int main(int argc, const char* argv[])
{

}
