#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
#include<aio.h>

#define BUFFER_SIZE 1025

int MAX_LIST = 2;


int main(int argc,char **argv)
{
    struct aiocb *listio[2];
    struct aiocb rd,wr;
    int fd,ret;

    //异步读事件
    fd = open("test1.txt",O_RDONLY);
    if(fd < 0)
    {
        perror("test1.txt");
    }

    bzero(&rd,sizeof(rd));

    rd.aio_buf = (char *)malloc(BUFFER_SIZE);
    if(rd.aio_buf == NULL)
    {
        perror("aio_buf");
    }

    rd.aio_fildes = fd;
    rd.aio_nbytes = 1024;
    rd.aio_offset = 0;
    rd.aio_lio_opcode = LIO_READ;   ///lio操作类型为异步读

    //将异步读事件添加到list中
    listio[0] = &rd;


    //异步些事件
    fd = open("test2.txt",O_WRONLY | O_APPEND);
    if(fd < 0)
    {
        perror("test2.txt");
    }

    bzero(&wr,sizeof(wr));

    wr.aio_buf = (char *)malloc(BUFFER_SIZE);
    if(wr.aio_buf == NULL)
    {
        perror("aio_buf");
    }

    wr.aio_fildes = fd;
    wr.aio_nbytes = 1024;

    wr.aio_lio_opcode = LIO_WRITE;   ///lio操作类型为异步写

    //将异步写事件添加到list中
    listio[1] = &wr;

    //使用lio_listio发起一系列请求
    ret = lio_listio(LIO_WAIT,listio,MAX_LIST,NULL);

    //当异步读写都完成时获取他们的返回值

    ret = aio_return(&rd);
    printf("\n读返回值:%d",ret);

    ret = aio_return(&wr);
    printf("\n写返回值:%d",ret);



    return 0;
}
