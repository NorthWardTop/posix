
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys.types.h>

extern Server g_pSvr

/*****************************************************************************
 函 数 名  : initSock
 功能描述  : 创建套接字, 并绑定
 输入参数  : const char* addr,int port
 输出参数  : int* skfd,
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月22日
    作    者   : my
    修改内容   : 新生成函数

*****************************************************************************/
int initSock(int* skfd,const char* addr,int port)
{
    int flg=0;
    struct sockaddr_in  
    //创建socket
    skfd=sock(AF_INET,SOCK_STREAM,0);
    bzero(&g_pSvr->svrAddr,sizeof(g_pSvr->));
    g_pSvr->svrAddr.sin_family = AF_INET;
    //字符串地址转换网络字节序地址
    inet_pton(AF_INET,addr,&g_pSvr->svrAddr.sin_addr.s_addr);
    g_pSvr->svrAddr.sin_port = htons(PORT);
    //bind addr info with global socket
    if(-1==bind(g_pSvr->svrFd, \
        (struct sockaddr*)&g_pSvr->svrAddr,sizeof(g_pSvr->svrAddr)))
    {
        perror("server socket bind error!\n");
        flg=-1;
    }
    //listen server Fd
    if(-1==listen(g_pSvr->svrFd,MAX_FD_SIZE))
    {
        perror("server socket listen error!\n");
        flg=-1;
    }
    
    return flg;
}



