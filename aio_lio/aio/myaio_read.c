#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <fcntl.h>
#include <aio.h>


#define BUF_ZISE    1024

int MAX_LIST=2;

int main(int argc, char const *argv[])
{
    //创建aio control block对象
    struct aiocb readBlk;
    int fd,ret,cnt;
    fd=open("test.txt",O_RDONLY);
    if(fd<0)
        perror("open test.txt error\n");

    bzero(&readBlk,sizeof(readBlk));
    readBlk.aio_buf=malloc(BUF_ZISE+1);//分配缓冲区
    readBlk.aio_fildes=fd;
    readBlk.aio_nbytes=BUF_ZISE;//字节大小
    

    return 0;
}
