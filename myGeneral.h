#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define handle_error(msg) do \
{ \
	
    printf("Code location:[file %s][line %d]",  __FILE__, __LINE__ ); \
    printf("Error No.:[%d][%s] , msg : %s\n", errno, strerror(errno), msg); \
    exit(EXIT_FAILURE); \
} \
while (0)


#define getnbit(bits, begin, n) (bits>>(begin-n+1) & ((1 << (n + 1)) - 1))