#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/shm.h>
#include <arpa/inet.h>
using namespace std;

struct pollfd

#define MAX_FD_SIZE   1024
#define PORT    	  6661
typedef struct ServerContext
{
	 int CliCnt;  //当前fd的个数
     int CliFds[MAX_FD_SIZE]; //当前fd的数组
     int MaxFd;
	 int iServerFd;  //监听句柄
	 struct timeval     tv;
	 struct sockaddr_in SockAdd; //服务器地址
	 fd_set AllFdSet;	 //所有FD_SETS的头 
}T_ServerContext;

typedef T_ServerContext * PT_ServerContext;
//定义服务端对象
static PT_ServerContext   g_ptSerContext;

/*****************************************************************************
 函 数 名  : main
 功能描述  : 主函数
 输入参数  : int argc
             char *argv[]
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月21日
    作    者   : chandler
    修改内容   : 新生成函数

*****************************************************************************/
int main( int argc, char *argv[] )
{	

	unsigned int i;
	//为服务端对象申请内存
	g_ptSerContext=(PT_ServerContext)malloc(sizeof(T_ServerContext));
	if(g_ptSerContext==NULL)
		{
		perror("malloc server object error\n");
		return 0;
	}
	//服务端对象初始化并设置
	memset(g_ptSerContext,0,sizeof(T_ServerContext));
	for(i=0;i<MAX_FD_SIZE;i++)
		{
		g_ptSerContext->CliFds[i] = -1;
	}
	//创建监听socket
	g_ptSerContext->iServerFd = socket(AF_INET,SOCK_STREAM,0);
	if(-1==g_ptSerContext->iServerFd)
		{
		perror("Creat server socket error\n");
		return 0;
	}
	//初始化地址信息
	g_ptSerContext->SockAdd.sin_family 		=AF_INET;
	g_ptSerContext->SockAdd.sin_port   		=htons(PORT);
	g_ptSerContext->SockAdd.sin_addr.s_addr =htonl(INADDR_ANY);
	//进行监听地址绑定
	if(bind(g_ptSerContext->iServerFd,(struct sockaddr *)&g_ptSerContext->SockAdd, \
		sizeof(g_ptSerContext->SockAdd))==-1)
		{
		perror("bind error\n");
		return 0;	
	}
	//设置监听
	if(listen(g_ptSerContext->iServerFd,MAX_FD_SIZE))
		{
		perror("listen error\n");
		return 0;	
	}
	//定义箱子的起始地址
	fd_set *ReadFdSet = &g_ptSerContext->AllFdSet;
	while(1)
	{
		int   iRet;
		//初始化箱子
		FD_ZERO(ReadFdSet);
	    //把监听句柄放入箱子
		FD_SET (g_ptSerContext->iServerFd,ReadFdSet);
	    g_ptSerContext->MaxFd = g_ptSerContext->iServerFd;
		for(i=0;i<MAX_FD_SIZE;i++)
		{
			int iTmpFd  = g_ptSerContext->CliFds[i];
			FD_SET(iTmpFd,ReadFdSet);
			g_ptSerContext->MaxFd  =(iTmpFd > g_ptSerContext->MaxFd ? iTmpFd:g_ptSerContext->MaxFd );
		}
		//设置select超时时间
		g_ptSerContext->tv.tv_sec  = 20;
		g_ptSerContext->tv.tv_usec = 0 ;
		//进行SELECT 轮询
		iRet=select(g_ptSerContext->MaxFd+1,ReadFdSet,NULL,NULL,&g_ptSerContext->tv);
		//是否为错误返回
		if(iRet == -1)
			{
			perror("select error\n");
			return 0;	
		}
		//是否为超时返回
		if(iRet==0)
			{
			printf("select time out!\n");
			continue ;
		}
		//有sk返回,判断返回fd是否为监听句柄
		if(FD_ISSET(g_ptSerContext->iServerFd,ReadFdSet))
		{
		   //为监听句柄
		   struct sockaddr_in Clinew;
		   socklen_t len = sizeof(Clinew);
		   int    iConFd= -1;
		   while(iConFd==-1)
		   	{
			 iConFd= accept(g_ptSerContext->iServerFd,(struct sockaddr *)&Clinew,&len);
			 if(iConFd ==-1)
			 {
				perror("accept error\n");
			 }
		   }
		   //将检测到的新链接的客户端，放入服务端的链接句柄数组中  
			for(i=0;i<MAX_FD_SIZE;i++)
			{
				//找到一个空位，存放新的FD
				if(g_ptSerContext->CliFds[i]<0)
					{
					g_ptSerContext->CliFds[i]=iConFd;
					g_ptSerContext->CliCnt++;
					break;
				}
			}
		 }
		 char 		aBuf[1024];
	     //有链接SK进行数据通信
	     for(i=0;i<g_ptSerContext->CliCnt;i++)
	     {
			int iTmpFd = g_ptSerContext->CliFds[i];
			if(iTmpFd <0)
				{	
				continue;
			}
			//是否当前fd有数据可读
			if(FD_ISSET(iTmpFd,ReadFdSet))
			{
			//可为新链接fork creat_thread新的进程/线程
				//iTmpFd可读
				int  iReadCnt = read(iTmpFd,aBuf,1024);
				if(iReadCnt<=0)
				{
				
					//read 发生错误
					FD_CLR(iTmpFd,&g_ptSerContext->AllFdSet);
					close(iTmpFd);
					g_ptSerContext->CliFds[i]=-1;
					continue;
				}
				printf("%s\n",aBuf);
			}
			//业务逻辑
			string sEchoBuf ="hello ,client";
			write(iTmpFd,sEchoBuf.c_str(),sEchoBuf.length()+1);
		 }
	}
    return 0;
}

