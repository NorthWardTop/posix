#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define handle_error(msg) do \
{ \
    printf("error No. : [%d][%s] , msg : %s\n", errno, strerror(errno), msg); \
    exit(EXIT_FAILURE); \
} \
while (0)