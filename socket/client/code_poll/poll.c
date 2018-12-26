#include "server.h"



extern PTServerST  g_ptServer;




/*****************************************************************************
 函 数 名  : DoPollHandle
 功能描述  : 1.完成poll操作的初始设置  2.进行poll操作  3.对poll的返回做响应
 输入参数  : int  listenfd
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月26日
    作    者   : chandler
    修改内容   : 新生成函数

*****************************************************************************/
void DoPollHandle( int  listenfd )
{
  unsigned int  i =0;
  int			iRet=0;
  struct sockaddr_in  tCliAddr;
  g_ptServer->Clientfds[0].fd 		= listenfd;
  g_ptServer->Clientfds[0].events	= POLLIN;
  //对当前的pollfd数组做初始化
  for(i=1;i<LISTENNUM;i++)
  	{
	 g_ptServer->Clientfds[i].fd     = -1;
  }
  g_ptServer->MaxFd  = 0;
  for(;;)
  	{
  		//进行poll的轮询
		iRet=poll(g_ptServer->Clientfds,g_ptServer->MaxFd+1,INFTIM);
		if(iRet==-1)
			{
			perror("poll error\n");
			exit(1);
		}
		//进行返回值的检查判断是否有fd准备好
		//判断当前监听句柄是否有新的链接
		if(g_ptServer->Clientfds[0].revent & POLLIN)
		{
			//监听到新的链接
			socklen_t len = sizeof(tCliAddr);
		    int    iConFd= -1;
		    while(iConFd==-1)
		   	{
				 iConFd= accept(listenfd,(struct sockaddr *)&tCliAddr,&len);
				 if(iConFd ==-1)
				 {
					perror("accept error\n");
				 }
		   }
		  fprintf(stdout,"Accept a new client: %s: %d\n",inet_ntoa(tCliAddr.sin_addr),tCliAddr.sin_port);
		  //将新链接入数组
		  for(i=1;i<LISTENNUM;i++)
		  	{
				if(g_ptServer->Clientfds[i].fd < 0)
				{
					g_ptServer->Clientfds[i].fd=iConFd;
					break;
				}		
		  }
		  if(i==LISTENNUM)
		  {
			fprintf(stderr,"MAX CLIENTS\n");
			exit(1);
		  }
		  g_ptServer->Clientfds[i].events = POLLIN;
		  g_ptServer->MaxFd= (i>g_ptServer->MaxFd ? i:g_ptServer->MaxFd);
		  //当前poll返回只有一个监听句柄 则循环结束
		  if(--iRet<=0)
		  	continue;
		}
		
		//处理客户端连接
		HandleClientConnection(g_ptServer->Clientfds,g_ptServer->MaxFd);
   }

}

















