/*
 * thread_work.c
 *
 *  Created on: 2015-4-16
 *      Author: hsl
 */
#include"thread_work.h"
#include"pub.h"

#include <sys/types.h>
#include <sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
//http 消息头
#define HEAD "HTTP/1.0 200 OK\n\
Content-Type: %s\n\
Transfer-Encoding: chunked\n\
Connection: Keep-Alive\n\
Accept-Ranges:bytes\n\
Content-Length:%d\n\n"
//http 消息尾
#define TAIL "\n\n"
extern char LOGBUF[1024];
//得到http 请求中 GET后面的字符串
void get_http_command(char *http_msg, char *command)
{
	char *p_end = http_msg;
	char *p_start = http_msg;
	while (*p_start) //GET /
	{
		if (*p_start == '/')
		{
			break;
		}
		p_start++;
	}
	p_start++;
	p_end = strchr(http_msg, '\n');
	while (p_end != p_start)
	{
		if (*p_end == ' ')
		{
			break;
		}
		p_end--;
	}
	strncpy(command, p_start, p_end - p_start);

}

//根据用户在GET中的请求，生成相应的回复内容
int make_http_content(const char *command, char **content)
{
	char *file_buf;
	int file_length;
	char headbuf[1024];

	if (command[0] == 0)
	{
		file_length = get_file_content("index.html", &file_buf);
	} else
	{
		file_length = get_file_content(command, &file_buf);
	}
	if (file_length == 0)
	{
		return 0;
	}

	memset(headbuf, 0, sizeof(headbuf));
	sprintf(headbuf, HEAD, get_filetype(command), file_length); //设置消息头

	int iheadlen = strlen(headbuf); //得到消息头长度
	int itaillen = strlen(TAIL); //得到消息尾长度
	int isumlen = iheadlen + file_length + itaillen; //得到消息总长度
	*content = (char *) malloc(isumlen); //根据消息总长度，动态分配内存
	if(*content==NULL)
	{
		memset(LOGBUF,0,sizeof(LOGBUF));
		sprintf(LOGBUF,"malloc failed %s\n", strerror(errno));
		save_log(LOGBUF);
	}
	char *tmp = *content;
	memcpy(tmp, headbuf, iheadlen); //安装消息头
	memcpy(&tmp[iheadlen], file_buf, file_length); //安装消息体
	memcpy(&tmp[iheadlen] + file_length, TAIL, itaillen); //安装消息尾
	//printf("headbuf:\n%s", headbuf);
	if (file_buf)
	{
		free(file_buf);
	}
	return isumlen; //返回消息总长度
}
void *http_thread(void *argc)
{
	//printf("thread begin \n");
	if(argc==NULL)
	{
		return NULL;
	}
	int st = *(int *) argc;
	free((int *)argc);
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	int rc = recv(st, buf, sizeof(buf), 0);
	if (rc <= 0)
	{
		memset(LOGBUF,0,sizeof(LOGBUF));
		sprintf(LOGBUF,"recv failed %s\n", strerror(errno));
		save_log(LOGBUF);
	} else
	{
		//printf("recv:\n%s", buf);
		char command[1024];
		memset(command, 0, sizeof(command));
		get_http_command(buf, command); //得到http 请求中 GET后面的字符串
		//printf("get:%s \n", command);
		char *content = NULL;
		int ilen = make_http_content(command, &content); //根据用户在GET中的请求，生成相应的回复内容
		if (ilen > 0)
		{
			send(st, content, ilen, 0); //将回复的内容发送给client端socket
			free(content);
		}
	}
	close(st); //关闭client端socket
	//printf("thread_is end\n");
	return NULL;

}
