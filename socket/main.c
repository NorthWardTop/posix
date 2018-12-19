
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

//流式套接字, 数据报套接字, 原始套接字
//头注释fi,
int main(int argc,char** argv)
{
    //printf("dsjadh");
    int skfd;//sock描述符
    int cnfd;//连接句柄
    int port;//端口号
    int addrLen=sizeof(struct sockaddr_in);//地址长度
    //服务器地址和客户端地址结构
    struct sockaddr_in srvAddr,cltAddr;
    
    char hello[]="hello, I am server";
    //参数不是2个或者端口号小与0
    if(2!=argc || ((port=atoi(argv[1]))<0))
    {
        printf("%s, port\n",argv[0]);
        exit(1);
    }

    //创建流式套接字
    skfd=socket(AF_INET,SOCK_STREAM,0);
    //设置服务器端地址结构
    srvAddr.sin_family=AF_INET;//ipv4协议族
    srvAddr.sin_addr.s_addr=htonl(INADDR_ANY);//转换为网络字节序
    srvAddr.sin_port=htons(port);//端口主机字节序->网络字节序
    //将套接字和地址信息绑定
    bind(skfd,(struct sockaddr*)(&srvAddr),sizeof(struct sockaddr));

    //监听,最大链接数为4
    listen(skfd,4);
    //
    while(1)
    {
        //接受客户端的链接,阻塞函数
        addrLen=sizeof(struct sockaddr_in);
        cnfd=accept(skfd,(struct sockaddr*)(&cltAddr),&addrLen);
        printf("connect from %s:%u ...\n",inet_ntoa(cltAddr.sin_addr),\
            ntohs(cltAddr.sin_port));
        write(cnfd,hello,strlen(hello));//向连接句柄中写入内容
        close(cnfd);//关闭连接
    }
    close(skfd);
    exit(0);
}























