

/*****************************************************************************
 函 数 名  : InitSocket
 功能描述  : 创建并初始化Socket ,进行绑定
 输入参数  : const char *
             int port
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月26日
    作    者   : chandler
    修改内容   : 新生成函数

*****************************************************************************/
int InitSocket(const char *ip, int port)
{
	int iListenFd;
	
	//创建socket
	iListenFd = socket(AF_INET,SOCK_STREAM,0);
	g_ptServer
    bzero(&g_ptServer->ServerAddr,sizeof(g_ptServer->ServerAddr));
	//初始化地址信息
	g_ptServer->ServerAddr.sin_family 		=AF_INET;
	inet_pton(AF_INET,ip,&g_ptServer->ServerAddr.sin_addr)
	g_ptServer->ServerAddr.sin_port   		=htons(port);
	if(bind(iListenFd,(struct sockaddr *)&g_ptServer->ServerAddr, \
		sizeof(g_ptSerContext->SockAdd))==-1)
	{
		perror("bind error\n");
		return 0;	
	}
	return  iListenFd;
}



