

PTServerST  g_ptServer;

/*****************************************************************************
 函 数 名  : InitServer
 功能描述  : 初始化服务端对象
 输入参数  : 无
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月26日
    作    者   : chandler
    修改内容   : 新生成函数

*****************************************************************************/
int InitServer(void)
{
    g_ptServer = (PTServerST)malloc(sizeof(ServerST));
	if(g_ptServer == NULL)
		{
		return -1;
	}
	memset(g_ptServer,0,sizeof(ServerST));
	return 0;
}





/*****************************************************************************
 函 数 名  : RemoveServer
 功能描述  : 进行客户端的销毁
 输入参数  : 无
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月26日
    作    者   : chandler
    修改内容   : 新生成函数

*****************************************************************************/
void RemoveServer( void  )
{
    if(g_ptServer)
    {
		free(g_ptServer);
		g_ptServer =NULL;
	}
}



/*****************************************************************************
 函 数 名  : PorcessClientData
 功能描述  : 客户端业务逻辑数据处理
 输入参数  : int clientfd
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年5月3日
    作    者   : chandler
    修改内容   : 新生成函数

*****************************************************************************/
int PorcessClientData(int clientfd)
{
    int  iRet;
	char aBuff[MAXLINE];
	iRet = read(clientfd,aBuff,MAXLINE);
	if(iRet ==0)
		{
	 	 close(clientfd);
		 return -1;
	}
	write(stdout,aBuff,iRet);
	//echo服务器
	write(clientfd,aBuff,iRet);
	return 0;
}


/*****************************************************************************
 函 数 名  : HandleClientConnection
 功能描述  : 服务器客户端数据处理函数
 输入参数  : struct pollfd *clientfds
             int num
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年5月3日
    作    者   : chandler
    修改内容   : 新生成函数

*****************************************************************************/
void HandleClientConnection( struct pollfd *clientfds, int num )
{
	unsigned int i=0;
	int 		 iRet=0;
    for(i=1;i<=num;i++)
    {
	 	if(clientfds[i].fd<0)
			continue;
		//判断当前客户端句柄是否有数据准备好
		if(clientfds[i].revents & POLLIN)
		{
		   	//客户端业务逻辑
		  	iRet=PorcessClientData(clientfds[i].fd);
			if(iRet==-1)
			{
				clientfds[i].fd = -1;
				continue;
			}
		}
	}
}








