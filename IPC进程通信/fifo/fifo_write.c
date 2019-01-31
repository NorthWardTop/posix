#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "myGeneral.h"


int main(int argc, char** argv)
{
    int ret,fifo_fd;
    char* buf="test message";

    if(access(FILE_NAME, F_OK)==-1) //当文件不存在时候
    {   //mkfifo
        ret=mkfifo(FILE_NAME, 0664);
        printf("%d", ret);
        if(ret!=0)
        {
            printf("%d:%s",errno,strerror(errno));
   //         handle_error("make fifo file err");
        }
    }
    fifo_fd=open("FILE_NAME", O_WRONLY);
    if(fifo_fd<0)
        handle_error("open fifo file err");
    //写
    ret=write(fifo_fd,buf,strlen(buf));
    if(ret<0)
        handle_error("Write fifo file err");
    printf("write process retun, pid: %d, write data: %s\n", getpid(), buf);
    return 0;
}

