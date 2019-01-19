


#define MAX_LISTENNUM   1024


typedef struct _Server
{
    struct sockaddr_in srvAddr;
    int                listenFd;
    int                epollFd;
}Server

Server* g_pSrv;

/*****************************************************************************
 函 数 名  : doEpoll
 功能描述  : 处理epoll
 输入参数  : int listenFd
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月26日
    作    者   : my
    修改内容   : 新生成函数

*****************************************************************************/
void doEpoll( int listenFd )
{
    int ret=0;
    //1.声明事件类型链表
    struct epoll_event events[EPOLL_EVT];
    //2.创建epoll对象
    g_pSrv->epollFd = epoll_creat(MAX_LISTENNUM);
    //3.添加监听事件(epfd,listenfd,Evt,timeout)
     (g_pSrv->epollFd,g_pSrv->listenFd,EPOLLIN,-1);
    //4.伦旭监听
    for(;;)
    {
        //5.获取已经返回的事件
        // int epoll_wait(int epfd, struct epoll_event *events,
        //              int maxevents, int timeout);
        ret=epoll_wait(g_pSrv->epollFd,events,EPOLL_EVT,-1);
        //6.对返回做处理
        
    }
}


/*****************************************************************************
 函 数 名  : epollRetProc
 功能描述  : 对epoll_wait的返回做处理
 输入参数  : int epfd,struct epoll_event* events,int num,int listenFd,char* buf
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月27日
    作    者   : my
    修改内容   : 新生成函数

*****************************************************************************/
void epollRetProc( int epfd,struct epoll_event* events, /
    int num,int listenFd,char* buf )
{
    unsigned int i=0;
    int fd=0;
    //遍历返回的事件
    for(i=0;i<num;++i)
    {
        fd=events[i].data.fd=listenFd;
        SIGABRT
    }
}






















