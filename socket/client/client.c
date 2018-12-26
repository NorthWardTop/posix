#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>


//#define PORT            8848
#define MAX_DATASIZE    1024

int main(int argc,char** argv)
{
    int skfd=-1,byteNum=0;
    char buf[MAX_DATASIZE];
    struct sockaddr_in svrAddr;

    if(argc<3 || atoi(argv[2])<=0)
    {
        fprintf(stderr,"%s: host port\n",argv[0]);
        exit(1);
    }

    skfd=socket(AF_INET,SOCK_STREAM,0);
    if(skfd==-1)
    {
        fprintf(stderr,"socket error\n");
        exit(1);
    }

    bzero(&svrAddr, sizeof(svrAddr));
    svrAddr.sin_family=AF_INET;//ipv4Ð­Òé×å
    inet_pton(AF_INET,argv[1],&svrAddr.sin_addr);
    svrAddr.sin_port=htons(argv[2]);

    
    

    while(1)
    {
        if(-1==connect(skfd,(struct sockaddr*)&svrAddr,sizeof(svrAddr)))
        fprintf(stderr,"connect error\n");
        byteNum=recv(skfd,buf,MAX_DATASIZE,0);
        if(-1==byteNum)
            continue;
        buf[byteNum]='\0';
        printf("received data from %s: %s \n",inet_ntoa(svrAddr.sin_addr),buf);
    }
}










