#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "myGeneral.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE    1024


int main(int argc, const char** argv)
{
    int fifo_fd;
    char buf[BUF_SIZE]="\0";
    int read_bytes;
    
    fifo_fd=open(FILE_NAME, O_RDONLY);
    if(fifo_fd<0)
        handle_error("open fifo file err");
    read_bytes=read(fifo_fd,buf,BUF_SIZE);
    if(read_bytes<0)
        handle_error("read fifo file err");
    printf("read process: %d, read fifo file data: %s\n", getpid(), buf);
    return 0;
}

