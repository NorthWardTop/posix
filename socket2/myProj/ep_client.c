#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <string.h>  
#include <errno.h>  
#include <netinet/in.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include  <arpa/inet.h>  
#include <sys/select.h>  
  
#define MAXLINE     1024    //最大缓冲区  
#define SERV_PORT   6000    //服务器端口号



void sendAndRecv(int connFd)
{
    FILE* fp=stdin;
    int lens;
    char sendBuf[MAXLINE];
    char recvBuf[MAXLINE];
    fd_set rset;
    FD_ZERO(&rset);
    int maxFd=(fileno(fp) > connFd ? fileno(fp) : connFd+1);
    //!> 注意不要把rset看作是简单的一个变量  
    //!> 注意它其实是可以包含一组套接字的哦，  
    //!> 相当于是封装的数组！每次都要是新的哦！ 
    while(1)
    {
        FD_SET(fileno(fp),&rset);
        FD_SET(connFd,&rset);

        if(select(maxFd, &rset,0,0,0)==-1)
            perror("client select error\n");
        
        //连接口有信息(from server msg)
        if(FD_ISSET(connFd, &rset))
        {
            printf("client get from server\n");
            memset(recvBuf,0,sizeof(recvBuf));
            int ret=read(connFd,recvBuf,MAXLINE);
            if(ret==0)
            {
                printf("recv OK\n");
                break;
            }
            else if(ret==-1)
            {
                printf("recv error\n");
                break;
            }
            else
            {
                lens=strlen(recvBuf);
                recvBuf[lens]='\0';
                write(STDOUT_FILENO,recvBuf,MAXLINE);//写到标准输出
                printf("\n");
            }
        }

        //标准输入stdin句柄有信息
        if(FD_ISSET(fileno(fp), &rset))
        {
            memset(sendBuf,0, sizeof(sendBuf));
            if(fgets(sendBuf,MAXLINE,fp)==NULL)
            {
                printf("End...\n");  
                exit( EXIT_FAILURE );  
            }
            else
            {
                lens=strlen(sendBuf);
                sendBuf[lens-1]='\0';//将回车换为\0
                if(strcmp(sendBuf,"q")==0)
                {
                    printf( "Bye..\n" );  
                    return;  
                }

                printf("Client send : %s\n", sendBuf);  
                write( connFd, sendBuf, strlen( sendBuf ) ); //写到链接句柄
            }
        }
    }

}

int main(int argc, char const *argv[])
{
    char buf[MAXLINE];
    int connFd;
    struct sockaddr_in srvAddr;

    if(argc!=2)
        perror("input server ip\n");
    
    connFd=socket(AF_INET,SOCK_STREAM,0);
    if(connFd==-1)
        perror("socket error\n");
    
    bzero(&srvAddr,sizeof(srvAddr));
    srvAddr.sin_family=AF_INET;
    srvAddr.sin_port=htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &srvAddr.sin_addr);

    if(connect(connFd,(struct sockaddr*)&srvAddr,sizeof(srvAddr))<0)
        perror("connect error\n");
    
    sendAndRecv(connFd);

    close(connFd);
    return 0;
}
