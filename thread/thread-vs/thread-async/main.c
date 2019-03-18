/*
线程同步,依赖于信号技术,信号被进程拥有
1.每个线程可向其他线程发送信号(pthread_kill信号)
2.每个线程可以设置本线程自己的信号屏蔽集合(被线程拥有),初始值是继承于父线程
3.同进程下所有的线程共享对某信号处理方法(执行最后一次设置的处理方法)
4.如果发送来的是终止信号, 则进程下所有线程终止
1.信号发送函数
	//当sig=0时仅仅检测线程是否存在,不发送信号,存在或成功返回0,不成功不存在返回errno
	int pthread_kill(pthread_t thread, int sig);
2.设置信号屏蔽掩码
	int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset);
	how: SIG_BLOCK(第二参数信号设置位添加屏蔽) SIG_UNBLOCK(第二参数中设置位解除屏蔽) SIG_SETMASK(直接设置)
	两个信号不能屏蔽和修改操作函数(SIGKILL SIGSTOP)
3.设置信号执行函数
	 int sigaction(int signum, const struct sigaction *act,
                     struct sigaction *oldact);


	

作业:
*/

#include <pthread.h>
#include <dksk>
#include <signal.h>

int main(int argc, const char* argv[])
{
	pthread_kill()
}

