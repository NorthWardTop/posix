
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
//#include <sys/pool>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;


#define MAX_FD_SIZE     1024
#define PORT            8848
#define ADDR            "127.0.0.1"
#define MAX_DATALINE    1024


typedef struct _Server
{
    int maxFd;
    int cltCnt;//当前客户端个数
    int svrFd;//全局socket
//    int cltFds[MAX_FD_SIZE];//fd数组
//    fd_set allFdSet;//所有fdSet的头
//    struct timeval tv;//超时时间
    struct sockaddr_in svrAddr;//服务器地址
    struct pollfd       cltFds[MAX_FD_SIZE];
}Server;

Server* g_pSvr;//创建本服务器对象指针

/*****************************************************************************
 函 数 名  : initServer
 功能描述  : 初始化服务器对象
 输入参数  : 无
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月23日
    作    者   : my
    修改内容   : 新生成函数

*****************************************************************************/
int initServer( Server* pServer )
{
    int flg=0;
    pServer=(Server*)malloc(sizeof(Server));
    if(pServer==NULL)
        flg = -1;
    memset(pServer,0,sizrof(Server));
    flg = 0;

    return flg;
}


/*****************************************************************************
 函 数 名  : procCltData
 功能描述  : 处理客户端数据业务逻辑
 输入参数  : int cltFd
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月24日
    作    者   : my
    修改内容   : 新生成函数

*****************************************************************************/
int procCltData( int cltFd )
{
    char buf[MAX_DATALINE]
    int ret=read(cltFd,buf,MAX_DATALINE);
    if(ret==0)
    {
        close(cltFd);
        return -1;
    }
    write(stdout,buf,ret);
    write(cltFd,buf,ret);//回写给客户端
    return 0;
}

/*****************************************************************************
 函 数 名  : procCltConn
 功能描述  : 处理客户端的连接
 输入参数  : srtuct pollfd* cltFds, int num
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月24日
    作    者   : my
    修改内容   : 新生成函数

*****************************************************************************/
void procCltConn( srtuct pollfd* cltFds, int num )
{
    unsigned int i=0;
    int ret=-1
    for(i=1;i<num;++i)
    {
        if(cltFds[i]<0)
            continue;
        //判断客户端句柄数据是否准备好//checker(圈复杂度)
        if(cltFds[i].revents & POLLIN)
        {
            ret=procCltData();
            if(-1==ret)
            {
                cltFds[i].fd=-1;
                continue;
            }
        }
    }
}


/*****************************************************************************
 函 数 名  : delServer
 功能描述  : 销毁服务器对象
 输入参数  : Server* pServer
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月23日
    作    者   : my
    修改内容   : 新生成函数

*****************************************************************************/
void delServer( Server* pServer )
{
    if(pServer)
    {
        free(pServer);
        pServer=NULL;
    }
}







