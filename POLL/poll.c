
/*****************************************************************************
 函 数 名  : doPoll
 功能描述  : 1.poll操作 2.poll的返回做出相应
 输入参数  : int listenFd
 输出参数  : 无
 返 回 值  : int 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月23日
    作    者   : my
    修改内容   : 新生成函数

*****************************************************************************/
int doPoll( int listenFd )
{
    int i=0,iRet,flg=0;
    socklen_t cltAddr;
    g_pSvr->cltFds[0].fd = listenFd;
    g_pSvr->cltFds[0].events = POLLIN;
    //对当前poll初始化
    for(int i=1;i<MAX_FD_SIZE;++i)
    {
        g_pSvr->cltFds[i].fd=-1;
    }
    g_pSvr->maxFd=0;
    
    for(;;)
    {
        //POLL轮询
        iRet=poll(g_pSvr->cltFds,g_pSvr->maxFd+1;INFTIM);
        if(iRet==-1)
        {
            flg=-1;
            perror("poll error!\n")
        }
        //返回检查
        if(g_pSvr->cltFds[0].events & POLLIN)
        {
            //监听到新连接
            int conFd=-1;//连接句柄
            socklen_t len=sizeof(cltAddr);//地址长度
            while(conFd==-1)//没有接受到连接就一直接收
            {
                conFd=accept(listenFd,(struct sockaddr*)&cltAddr,&len);
                if(conFd==-1)
                    perror("accept error!\n");
            }
            fprintf(stdout,"Accept a new clt...");
            fprintf(stdout,"from     %s:%d",inet_ntoa(cltAddr.sin_addr),cltAddr.sin_port);
            //加入数组
            for(i=1;i<MAX_FD_SIZE;++i)
            {
                if(g_pSvr->cltFds[i].fd<0)//空闲
                {
                    g_pSvr->cltFds[i].fd=conFd;//客户端连接句柄
                    g_pSvr->cltFds[i].events=POLLIN;//为输入事件
                    g_pSvr->maxFd=(i>g_pSvr->maxFd)?i:g_pSvr->maxFd;
                }
            }
            if(i==MAX_FD_SIZE)
            {
                fprintf(stderr, "Out of MAX_FD_SIZE");
                flg=-1;
            }
            //当前只返回一个监听句柄
            if(iRet==1 | iRet==0)
                continue;
            
            procCltConn(g_pSvr->cltFds, g_pSvr->maxFd)
        }
    }
}















