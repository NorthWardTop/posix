#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <string.h>  
#include <sys/types.h>  
#include <errno.h>  
#include <sys/socket.h>  
#include <netinet/in.h>           /* socket�ඨ����Ҫ*/  
#include <sys/epoll.h>            /* epollͷ�ļ� */  
#include <fcntl.h>                    /* nonblocking��Ҫ */  
#include <sys/resource.h>     /* ����������������Ҫsetrlimit */  
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>

#define MAXEPOLL        100        //最大ep的客户端数量        
#define MAXLINE         1024       //最大发送长度
#define PORT            6000       //本地服务端口
#define MAXBACK         100        //最大链接数


// int setnonblocking( int fd )  
// {  
//     if( fcntl( fd, F_SETFL, fcntl( fd, F_GETFD, 0 )|O_NONBLOCK ) == -1 )  
//     {  
//         printf("Set blocking error : %d\n", errno);  
//         return -1;  
//     }  
//     return 0;  
// }  


void processData(int cltFd)
{
    static int cltCnt;
    int pid;
    int recvLen;
    char recvBuf[MAXLINE];

    cltCnt++;
    printf("connect from client %d\n",cltCnt);
    //子进程接受客户端发来的消息, 并回射
    if(pid=fork()==0)
    {
        while(1)
        {
            recvLen=recv(cltFd, recvBuf,MAXLINE,0);
            if(recvLen<=0)
            {
                close(cltFd);
                exit(0);
            }
            else
            {
                recvBuf[recvLen]='\0';
                printf("Msg from Client %d :%s\n", cltCnt,recvBuf);
                write(cltFd,recvBuf,recvLen);
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    int listenFd, connFd, epollFd;
    int curFds;//当前已存在的数量
    int waitFds;//ep_wait返回值

    int i;
    struct sockaddr_in srvAddr,cltAddr;
    struct epoll_event ev;
    struct epoll_event evs[MAXEPOLL];
    struct rlimit rlt;

    char buf[MAXLINE];
    socklen_t len=sizeof(struct sockaddr_in);//地址长度
    //!> 设置每个进程允许打开的最大文件数  
    //!> 每个主机是不一样的哦，一般服务器应该很大吧！
    rlt.rlim_max=rlt.rlim_cur=MAXEPOLL;
    if(setrlimit(RLIMIT_NOFILE,&rlt)==-1)
        perror("set rlimit error\n");
    //设置服务端套接字
    bzero(&srvAddr,sizeof(srvAddr));//服务端地址置零;
    srvAddr.sin_family=AF_INET;
    srvAddr.sin_addr.s_addr=htonl(INADDR_ANY);//任何地址都允许通过
    srvAddr.sin_port=htons(PORT);//host to net for short

    if((listenFd=socket(AF_INET,SOCK_STREAM,0))==-1)
        perror("socket error\n");
    //设置非阻塞模式
    int tmpFd=fcntl(listenFd, F_GETFD,0) ;
    if(fcntl(listenFd, F_SETFL, tmpFd| O_NONBLOCK)==-1)
        perror("setnonblocking error\n");
   


    if(bind(listenFd,(struct sockaddr*)&srvAddr,sizeof(struct sockaddr))==-1)
        perror("bind error\n");
    if(listen(listenFd,MAXBACK)==-1)
        perror("listen error\n");
    
    //创建一个epool实例
    epollFd=epoll_create(MAXEPOLL);
    //设置事件对象
    ev.events=EPOLLIN | EPOLLET;//允许哪些事件
    ev.data.fd=listenFd;//
    //添加到事件epFd
    if(epoll_ctl(epollFd,EPOLL_CTL_ADD,listenFd,&ev)<0)
        perror("epoll_ctl add 1 error\n");
    curFds=1;

    while(1)
    {
        //等待返回
        waitFds=epoll_wait(epollFd,evs,curFds,-1);
        if(waitFds==-1)
            perror("epoll wait error\n");

        //处理已经返回的所有fd
        for(i=0;i<waitFds;i++)
        {
            //监听端口有事  
            if(evs[i].data.fd == listenFd && curFds < MAXEPOLL)
            {
                connFd=accept(listenFd,(struct sockaddr*)&cltAddr,&len);
                if(connFd==-1)
                    perror("accept error\n");
                printf("Accept a client!\n");

                //加入已连接队列
                ev.events=EPOLLIN | EPOLLET;
                ev.data.fd=connFd;
                if(epoll_ctl(epollFd,EPOLL_CTL_ADD,connFd,&ev)<0)
                    perror("epoll_ctl add 2 error\n");
                curFds++;
                continue;
            }
            //不是链接语句返回.则处理用户数据
            processData(evs[i].data.fd);
        }
    }


    close(listenFd);

    return 0;
}
