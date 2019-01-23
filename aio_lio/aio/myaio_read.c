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


void errInfo(const char* errMsg)
{
    printf("error No:%d, error msg:%s\n",errno, errMsg);
}

int main(int argc, char const *argv[])
{
    //创建aio control block对象
    struct aiocb readBlk;
    int fd,ret,cnt=0;
    fd=open("test.txt",O_RDONLY);
    if(fd<0)
        printf("open test.txt error\n");

    bzero(&readBlk,sizeof(readBlk));
    readBlk.aio_buf=malloc(BUF_ZISE+1);//分配缓冲区
    readBlk.aio_fildes=fd;//将要操作的句柄
    readBlk.aio_nbytes=BUF_ZISE;//字节大小
    readBlk.aio_offset=0;//偏移为0
    //进行异步读;
    ret=aio_read(&readBlk);
    if(ret<0)
        errInfo("aio err");

    // EINPROGRESS  I/O还没完成
    // ECANCELLED	 I/O被取消
    // -1           发生错误 
    // 正常		I/O已处理完成 
    while(aio_error(&readBlk)==EINPROGRESS)
        printf("%d次\t",cnt++);
    printf("\n");
    //获取异步读内容
    ret=aio_return(&readBlk);
    printf("aio_return:%d\n\n", ret);
    printf("aio_buf:%s\n",(char*)readBlk.aio_buf);

    free(readBlk.aio_buf);
    readBlk.aio_buf=NULL;
    close(fd);
    
    return 0;
}
