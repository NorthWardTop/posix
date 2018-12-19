#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
int main(int argc, char *argv[])
{
    int skfd,cnfd,addr_len;
    struct sockaddr_in srv_addr,clt_addr;
    int portnumber;
    char hello[]="Hello! Long time no see.\n";
    if(2 != argc || 0 > (portnumber=atoi(argv[1])))
    {
         printf("Usage:%s port\n",argv[0]);
         exit(1);
    }
    
    /* 创建IPv4的流式套接字描述符 */
    if(-1 == (skfd=socket(AF_INET,SOCK_STREAM,0)))
    {
         perror("Socket Error:");
         exit(1);
    }

    /* 填充服务器端sockaddr地址结构 */
    bzero(&srv_addr,sizeof(struct sockaddr_in));
    srv_addr.sin_family=AF_INET;
    srv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    srv_addr.sin_port=htons(portnumber);

    /* 将套接字描述符skfd和地址信息结构体绑定起来 */
    if(-1 == bind(skfd,(struct sockaddr *)(&srv_addr),sizeof(struct sockaddr)))
    {
         perror("Bind error:");
         exit(1);
    }

    /* 将skfd转换为被动建通模式 */
    if(-1 == listen(skfd,4))
    {
         perror("Listen error:");
         exit(1);
    }

    while(1)
    {
       /* 调用accept,服务器端一直阻塞，直到客户程序与其建立连接成功为止*/
        addr_len=sizeof(struct sockaddr_in);
        if(-1 == (cnfd=accept(skfd,(struct sockaddr *)(&clt_addr),&addr_len)))
        {
             perror("Accept error:");
             exit(1);
        }
        printf("Connect from %s:%u ...!\n",inet_ntoa(clt_addr.sin_addr),ntohs(clt_addr.sin_port)); 
        if(-1 == write(cnfd,hello,strlen(hello))){
             perror("Send error:");
             exit(1);
        }
        close(cnfd);
     }
     close(skfd);
     exit(0);
}