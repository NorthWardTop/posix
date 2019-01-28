/*
 * server.c
 *
 *  Created on: 2015-3-16
 *      Author: hsc
 */

#include<stdio.h>
#define BUFFSIZE 1024
#include"thread_work.h"
#include"pub.h"
int main(int argc,char *argv[])
{
	if(argc<2)
	{
		printf("usage:server port \n");
		return 0;
	}
	int port = atoi(argv[1]);

	if(port <= 0)
	{
		printf("port must be positive integer: \n");
		return 0;
	}

	int st =socket_create(port);
	if(st==0)
	{
		return 0;
	}
	setdaemon(); //设置为守护进程
	printf("my http server begin\n");
	socket_accept(st);
	close(st);
}
