#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <iostream>

typedef struct _Client
{
    int skfd;//sock 本机sock描述符
    int confd;//链接sock描述符
    struct sockaddr_in addr;//本机地址
    struct hostent* host;//远程地址
    short int port;//端口
    
}Client;

Client* g_pClt;

int main(int argc,char** argv)
{
    if(argc<2)
    {
        perror("%s port!\n",argv[0]);
        exit(1);
    }
    //创建对象
    g_pClt=(Client*)malloc(sizeof(Client));
    //清空并设置对象
    memset(g_pClt,NULL,sizeof(Client));
    //设置sock
    g_pClt->skfd = socket(AF_INET,SOCK_STREAM,0);
    if(-1==g_pClt->skfd)
    {
        perror("create sock error!");
        exit(1);
    }
    //设置地址
    g_pClt->addr.sin_family = AF_INET;
    g_pClt->addr.sin_port = htons(argv[1]);
    g_pClt->addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind地址信息到sock
//    if(-1==bind(g_pClt->skfd,(struct sockaddr*)&g_pClt->addr,sizeof(struct sockaddr)))
//    {
//        perror("bind sock addr error!");
//        exit(1);
//    }
    //连接
    if(-1==connect(g_pClt->skfd,(struct sockaddr*)&
    

    
    
































