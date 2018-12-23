
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
        iRet=poll(g_pSvr->cltFds,g_pSvr->maxFd+1;INFTIM);
        if(iRet==-1)
        {
            flg=-1;
            perror("")
    
}

