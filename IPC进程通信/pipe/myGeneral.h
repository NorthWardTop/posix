#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define handle_error(msg)   \
    do                      \
    {                       \
        printf("error No. : %d , msg : %s\n", errno, msg);        \
        exit(EXIT_FAILURE); \
    } while (0)