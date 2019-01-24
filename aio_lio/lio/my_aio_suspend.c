#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <aio.h>

#define BUF_SIZE    1024

int main(int argc, char const *argv[])
{
    struct aiocb* lio[2], rd, wr;
    lio[0]=&rd;
    lio[1]=&wr;
    char* buf[BUF_SIZE];

    //将文本用aioread从test.txt中读入到buf中, 


    aio_suspend()
    
    return 0;
}
