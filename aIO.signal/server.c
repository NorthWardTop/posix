#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


#define MAXSIZE		1024
#define LISTENNUM	1024
#define IP 			127.0.0.1
#define PORT 		8848

typedef struct _Server{
  struct sockaddr_in ServerAddr;
  int   			 listenfd;
  struct sigaction   handler;//信号子对象
  
}Server;

Server* g_pSvr;

//³õÊ¼»¯·þÎñÆ÷
int svrInit(Server* pSvr)
{
    pSvr=(Server*)malloc(sizeof(Server));
    memset(pSvr,NULL,sizeof(Server));
    if(pSvr)
        return 0;
    return -1;
}

/* Structure to contain information about address of a service provider.  */
//struct addrinfo
//{
//  int ai_flags;			/* Input flags.  */
//  int ai_family;		/* Protocol family for socket.  */
//  int ai_socktype;		/* Socket type.  */
//  int ai_protocol;		/* Protocol for socket.  */
//  socklen_t ai_addrlen;		/* Length of socket address.  */
//  struct sockaddr *ai_addr;	/* Socket address for socket.  */
//  char *ai_canonname;		/* Canonical name for service location.  */
//  struct addrinfo *ai_next;	/* Pointer to next in list.  */
//};

//³õÊ¼»¯socket
int sockInit(const char* ip,int port)
{
    int skfd=0;
    struct addrinfo ai;
    memset(&ai,0,sizeof(ai));

    ai.ai_family    =AF_UNSPEC;//
    ai.ai_flags     =AI_PASSIVE;//
    ai.ai_socktype  =SOCK_DGRAM;//Êý¾Ý±¨Ì×½Ó×Ö
    ai.ai_protocol  =IPPROTO_UDP;//UDP·þÎñÆ÷
    //ע��ָ��
    struct addrinfo* svrAddr;
    int ret=getaddrinfo(0,0,&ai,&svrAddr);
   	if(ret<0)
   		perror("Get address information error!\n");
   	//协议族,sock类型,协议
   	skfd=socket(svrAddr->ai_family,svrAddr->ai_socktype,svrAddr->ai_protocol);
   	if(bind(skfd,svrAddr->ai_addr,svrAddr->ai_addrlen)<0)
   	{
   		perror("bind error!\n");
   		return -1;
   	}

   	freeaddrinfo(svrAddr);
   	//freeaddrinfo(ai);
   	return skfd;
}

void onSignal(int sigNum)
{
    do
    {
        char buf[MAXSIZE];
        struct sockaddr_storage cltAddr;
        ssize_t recvSize=0,sendSize=0;
        size_t cltAddrLen=sizeof(cltAddr);
        recvSize=recvfrom(g_pSvr->listenfd,buf,MAXSIZE,
            0,(struct sockaddr*)&cltAddr,&recvSize);
        if(recvSize<0)
        {
            if(errno!=EWOULDBLOCK)
            {
                perror("recvfrom error\n");
            }
            else
            {
                printf("proc client\n");
                sendSize=sendto(g_pSvr->listenfd,buf,
                    (struct sockaddr*)&cltAddr,sizeof(cltAddr));
                if(sendSize<0)
                    perror("sendto error\n");
                else if(sendSize==recvSize)
                    perror("send unexpected number of bytes\n");
            }
        
    }while(recvSize>=0)
}


int main()
{
    int skFd,connFd,ret;
    if(svrInit()<0)
    {
    	perror("Init Server Error!\n");
    	return -1;
    }
    g_pSvr->listenfd=sockInit(IP,PORT);
    if(listen(g_pSvr->listenfd,LISTENNUM)<0)
    {
    	perror("Listen error!\n");
    	return -1;
    }
    //设置信号处理函数
    g_pSvr->handler.sa_handler=onSignal;
     
    if(sigfillset(&g_pSvr->handler.sa_mask)<0)
    {
        perror("signal fill error\n");
        return -1;
    }

    g_pSvr->handler.sa_flags=0;
    //ʹ���ź�
    if(sigaction(SIGIO,&g_pSvr->handler,0)<0)
    {
        perror("sigaction error\n");
        return -1;
    }

    //1.�źŲ����󷢸�˭(˭,����ӵ��, ��pid)
    if(fcntl(g_pSvr->listenfd, F_SETOWN,getpid())<0)
    {
        perror("set owner error!\n");
        return -1;
    }

    //2.�߱��źŽ�������(lsFd,
    if(fcntl(g_pSvr->listenfd, F_SETFL, O_NONBLOCK|FASYNC)<0)
    {
        perror("Get the file access mode and the file status flags error!\n");
        return -1;
    }
    
    
    return 0;
}
















