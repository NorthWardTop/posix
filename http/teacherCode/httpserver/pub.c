/*

 * pub.c
 *
 *  Created on: 2015-4-16
 *      Author: hsc
 */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<arpa/inet.h>

#include"pub.h"
#include"thread_work.h"
char LOGBUF[1024];
void save_log(char *buf)
{
      FILE *fp = fopen("log.txt","a+");  
      fputs(buf,fp);  
      fclose(fp);  
}
void setdaemon() //设置为守护进程
{
	pid_t pid, sid;
	pid = fork();
	if (pid < 0)
	{
		memset(LOGBUF,0,sizeof(LOGBUF));
		sprintf(LOGBUF,"fork failed %s\n", strerror(errno));
		save_log(LOGBUF);
		exit (EXIT_FAILURE);
	}
	if (pid > 0)
	{
		exit (EXIT_SUCCESS);
	}

	if ((sid = setsid()) < 0)
	{
		printf("setsid failed %s\n", strerror(errno));
		exit (EXIT_FAILURE);
	}
  /*if (chdir("/") < 0)
	 {
	 printf("chdir failed %s\n", strerror(errno));
	 exit(EXIT_FAILURE);
	 }*/
	 umask(0);
	 close(STDIN_FILENO);
	 close(STDOUT_FILENO);
	 close(STDERR_FILENO);

}

const char *get_filetype(const char *filename) //根据扩展名返回文件类型描述
{
	////////////得到文件扩展名///////////////////
	char sExt[32];
	const char *p_start=filename;
	memset(sExt, 0, sizeof(sExt));
	while(*p_start)
	{
		if (*p_start == '.')
		{
			p_start++;
			strncpy(sExt, p_start, sizeof(sExt));
			break;
		}
		p_start++;
	}

	////////根据扩展名返回相应描述///////////////////

	if (strncmp(sExt, "bmp", 3) == 0)
		return "image/bmp";

	if (strncmp(sExt, "gif", 3) == 0)
		return "image/gif";

	if (strncmp(sExt, "ico", 3) == 0)
		return "image/x-icon";

	if (strncmp(sExt, "jpg", 3) == 0)
		return "image/jpeg";

	if (strncmp(sExt, "avi", 3) == 0)
		return "video/avi";

	if (strncmp(sExt, "css", 3) == 0)
		return "text/css";

	if (strncmp(sExt, "dll", 3) == 0)
		return "application/x-msdownload";

	if (strncmp(sExt, "exe", 3) == 0)
		return "application/x-msdownload";

	if (strncmp(sExt, "dtd", 3) == 0)
		return "text/xml";

	if (strncmp(sExt, "mp3", 3) == 0)
		return "audio/mp3";

	if (strncmp(sExt, "mpg", 3) == 0)
		return "video/mpg";

	if (strncmp(sExt, "png", 3) == 0)
		return "image/png";

	if (strncmp(sExt, "ppt", 3) == 0)
		return "application/vnd.ms-powerpoint";

	if (strncmp(sExt, "xls", 3) == 0)
		return "application/vnd.ms-excel";

	if (strncmp(sExt, "doc", 3) == 0)
		return "application/msword";

	if (strncmp(sExt, "mp4", 3) == 0)
		return "video/mpeg4";

	if (strncmp(sExt, "ppt", 3) == 0)
		return "application/x-ppt";

	if (strncmp(sExt, "wma", 3) == 0)
		return "audio/x-ms-wma";

	if (strncmp(sExt, "wmv", 3) == 0)
		return "video/x-ms-wmv";

	return "text/html";
}

int socket_create(int port)
{
	int st = socket(AF_INET, SOCK_STREAM, 0);
	int on =1;
	if (st == -1)
	{
		memset(LOGBUF,0,sizeof(LOGBUF));
		sprintf(LOGBUF,"%s,%d:socker error %s\n", __FILE__, __LINE__, strerror(errno));
		save_log(LOGBUF);
		return 0;
	}
	if (setsockopt(st, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
		{
			memset(LOGBUF,0,sizeof(LOGBUF));
			sprintf(LOGBUF,"setsockopt failed %s\n", strerror(errno));
			save_log(LOGBUF);
			return 0;
		}
	struct sockaddr_in sockaddr;
	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_port = htons(port); //指定一个端口号并将hosts字节型传化成Inet型字节型（大端或或者小端问题）
	sockaddr.sin_family = AF_INET;	//设置结构类型为TCP/IP
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);	//服务端是等待别人来连，不需要找谁的ip
	//这里写一个长量INADDR_ANY表示server上所有ip，这个一个server可能有多个ip地址，因为可能有多块网卡
	if (bind(st, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) == -1)
	{
		memset(LOGBUF,0,sizeof(LOGBUF));
		sprintf(LOGBUF,"%s,%d:bind error %s \n", __FILE__, __LINE__, strerror(errno));
		save_log(LOGBUF);
		return 0;
	}

	if (listen(st, 100) == -1) // 	服务端开始监听
	{
		memset(LOGBUF,0,sizeof(LOGBUF));
		sprintf(LOGBUF,"%s,%d:listen failture %s\n", __FILE__, __LINE__,
				strerror(errno));
				save_log(LOGBUF);
		return 0;
	}
	printf("start server success!\n");
	return st;

}
int socket_accept(int st)
{
	int client_st;
	struct sockaddr_in client_sockaddr;
	socklen_t len = sizeof(client_sockaddr);

	pthread_t thrd_t;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED); //初始化线程为可分离的
	memset(&client_sockaddr, 0, sizeof(client_sockaddr));

	while (1)
	{
		client_st = accept(st, (struct sockaddr *) &client_sockaddr, &len);
		if (client_st == -1)
		{
			memset(LOGBUF,0,sizeof(LOGBUF));
			sprintf(LOGBUF,"%s,%d:accept failture %s \n", __FILE__, __LINE__,
					strerror(errno));
			save_log(LOGBUF);
			return 0;
		} else
		{
			int *tmp = (int *) malloc(sizeof(int));
			*tmp = client_st;
			pthread_create(&thrd_t, &attr, http_thread, tmp);
		}

	}
	pthread_destory(&attr);//释放资源
}

int  get_file_content(const char *file_name, char **content) // 得到文件内容
{
	int  file_length = 0;
	FILE *fp = NULL;

	if (file_name == NULL)
	{
		return file_length;
	}

	fp = fopen(file_name, "rb");

	if (fp == NULL)
	{
		memset(LOGBUF,0,sizeof(LOGBUF));
		sprintf(LOGBUF,"file name: %s,%s,%d:open file failture %s \n",file_name, __FILE__, __LINE__,
				strerror(errno));
		save_log(LOGBUF);
		return file_length;
	}

	fseek(fp, 0, SEEK_END);
	file_length = ftell(fp);
	rewind(fp);

	*content = (char *) malloc(file_length);
	if (*content == NULL)
	{
		memset(LOGBUF,0,sizeof(LOGBUF));
		sprintf(LOGBUF,"%s,%d:malloc failture %s \n", __FILE__, __LINE__,
				strerror(errno));
		save_log(LOGBUF);
		return 0;
	}
	fread(*content, file_length, 1, fp);
	fclose(fp);

	return file_length;
}
