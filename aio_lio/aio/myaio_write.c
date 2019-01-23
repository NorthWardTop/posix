#include <stdio.h>
#include <stdlib.h>
#include <aio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


#define BUF_ZISE    1024

void errInfo(const char* errMsg)
{
    fprintf(stdout,"error No:%d, msg:%s\n",errno, errMsg);
}

int main(int argc, char const *argv[])
{
    int fd,ret;
    char* str="hellllllllllllo, woooooooooooooooooooorld";
    struct aiocb wr;

    fd=open("test2.txt",O_WRONLY | O_APPEND);//文件打开为只写和追加模式
    if(fd<0)
        errInfo("open test2.txt err");
        
    //设置aio control block对象
    //wr.aio_offset=0;已经设置追加模式, 无需设置偏移量
    wr.aio_buf=(char*)malloc(BUF_ZISE);
    wr.aio_fildes=fd;
    wr.aio_nbytes=BUF_ZISE;
    if(wr.aio_buf==NULL)
        errInfo("aio_buf malloc err");
    wr.aio_buf=str;
    ret=aio_write(&wr);//异步写入
    if(ret<0)
        errInfo("aio_write err");
    
    //查看写状态
    while(ret=aio_error(&wr)==EINPROGRESS);//处理中(!=0)
 //       printf("in process...");
    
    ret=aio_return(&wr);//查看处理结果
    printf("\nresult of handling:%d\n",ret);

    close(fd);
   // system("cat test2.txt");

    return 0;
}
