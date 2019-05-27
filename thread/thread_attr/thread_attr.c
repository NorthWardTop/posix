/**
 * 线程属性,增加代码移植性, 规范性
 * pthread_attr_init初始化线程属性对象pthread_attr_t
 * pthread_attr_destroy销毁属性
pthread_attr_setaffinity_np
pthread_attr_setdetachstate
线程保护区(防止栈溢出)大小=PAGESIZE*n,=0不设置, 设置会导致资源浪费
结构体巨大, 设置溢出保护区
pthread_attr_setguardsize 
pthread_attr_setinheritsched
pthread_attr_setschedparam
pthread_attr_setschedpolicy
pthread_attr_setscope
pthread_attr_setstack 
pthread_attr_setstackaddr 栈地址
pthread_attr_setstacksize 栈大小

pthread_attr_getaffinity_np
pthread_attr_getdetachstate
pthread_attr_getguardsize
pthread_attr_getinheritsched
pthread_attr_getschedparam
pthread_attr_getschedpolicy
pthread_attr_getscope
pthread_attr_getstack
pthread_attr_getstackaddr
pthread_attr_getstacksize
===============================高级部分================================
1.守护进程: xinetd
作用: 使后台运行, 不是终端子进程
步骤:	创建守护进程, create_daemon, 将当前main函数进程设置为守护进程
		1.umask(0),设置文件模式创建屏蔽字设置为0
		2.fork, 并退出父进程exit(0)
		3.设置新会话setsid()
		4.SIGCHLD设置为忽略信号,子进程退出无需提醒父进程
		5.fork, 并退出父进程
		6.更改工作目录到根目录
		7.关闭输入, 重定向输出错误到/dev/null设备, close(0), dup2(fd,1), dup2(fd,1)
2. xinetd为系统提供服务,可以设置进程为守护进程
	配置文件/etc/xinetd.conf中添加
	instances=n,最多守护60守护进程
	includedir /etc/xinetd.d 指定守护进程文件夹
3. 时间服务器 

 */


#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>

int create_daemon()
{
	int fd, ret=-1;
	pid_t pid;
	struct sigaction sa;
	//将文件模式创建屏蔽字设置为0
	umask(0);

	//创建新进程
	if ((id = fork()) < 0) {
		printf("fork failed!\n");
		return -errno;
	} else if (id != 0) {
		exit(0); //退出父进程, 不和终端关联
	}
	//从这里开始只有子进程
	if ((id = fork()) < 0) {
		printf("fork failed!\n");
		return -errno;
	} else if(id != 0) {
		//如果是父进程, 则退出
		exit(0);
	}

	//从这里开始只有孙进程
	//更改工作目录到根目录
	if (chdir("/") < 0) {
		printf("change dir failed!\n");
		return -errno;
	}

	//关闭输入, 重定向输出和错误到/dev/null设备
	close(0);
	fd = open("/dev/null", O_RDONLY);
	dup2(fd, 1);
	dup2(fd, 2);
}


int main()
{
	int ret = create_daemon();
	if (ret < 0) {
		perror("create daemon failed!\n");
		return ret;
	}

	while (1) {
		sleep(1);
	}

	return 0;
}

