#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define handle_error(msg) do \
{ \
    printf("code location:[file %s][line %d]",  __FILE__, __LINE__ ); \
    printf("error No.:[%d][%s] , msg : %s\n", errno, strerror(errno), msg); \
    exit(EXIT_FAILURE); \
} \
while (0) 

#define FILE_NAME   "/mnt/hgfs/SI/LinuxkernelStu/fft"
