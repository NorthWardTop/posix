#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <linux/in.h>

#define SOCKET_OPS_BASE          128  
#define SOCKET_OPS_SET       (SOCKET_OPS_BASE)  
#define SOCKET_OPS_GET      (SOCKET_OPS_BASE)  
#define SOCKET_OPS_MAX       (SOCKET_OPS_BASE + 1)  


#define UMSG      "a message from userspace/n"
#define UMSG_LEN  sizeof("a message from userspace/n")

char kmsg[64];

int main(void)
{
  int sockfd;
  int len;

  sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
  if(sockfd < 0)
    {
      printf("can not create a socket/n");
      return -1;
    }

  /*call function data_to_kernel()*/
  setsockopt(sockfd, IPPROTO_IP, SOCKET_OPS_SET, UMSG, UMSG_LEN);

  len = sizeof(char)*64;

  /*call function data_from_kernel()*/
  getsockopt(sockfd, IPPROTO_IP, SOCKET_OPS_GET, kmsg, &len);
  printf("kmsg: %s", kmsg);

  close(sockfd);
  return 0;
}

 