#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <aio.h>
#include <sys/stat.h>
#include <fcntl.h>


#define BUF_SIZE    1024
#define LIST_SIZE   2

void errInfo(const char* errMsg)
{
    printf("error No:%d, message:%s\n",errno, errMsg);
    exit(0);
}


int main(int argc, char const *argv[])
{
    struct aiocb* listio[LIST_SIZE];
    struct aiocb rd,wr;
    int fd,ret;
    
    fd=open("test.txt", O_RDONLY);
    if(fd<0)
        errInfo("open test.txt err");
    bzero(&rd,sizeof(rd));
    rd.aio_buf=(char*)malloc(BUF_SIZE);
    rd.aio_fildes=fd;
    rd.aio_offset=0;
    rd.aio_nbytes=BUF_SIZE;
    rd.aio_lio_opcode=LIO_READ;
    if(rd.aio_buf==NULL)
        errInfo("rd alloc buf memory err");
    listio[0]=&rd;
    
    fd=open("test2.txt", O_WRONLY | O_APPEND);
    if(fd<0)
        errInfo("open test2.txt err");
    bzero(&wr,sizeof(wr));
    wr.aio_lio_opcode=LIO_WRITE;
    wr.aio_nbytes=BUF_SIZE;
    wr.aio_fildes=fd;
    wr.aio_buf=(char*)malloc(BUF_SIZE);
    if(wr.aio_buf==NULL)
        errInfo("wr alloc buf memory err");
    listio[1]=&wr;

    //LIO_WAIT为阻塞等待全部结束后返回
    ret=lio_listio(LIO_WAIT,listio,LIST_SIZE,NULL);
    if(ret==0)
        printf("all of the I/O operations have completed successfully.");

    ret = aio_return(&rd);
    printf("\n读返回值:%d,%s",ret,rd.aio_buf);

    ret = aio_return(&wr);
    printf("\n写返回值:%d,%s",ret, wr.aio_buf);

    return 0;
}