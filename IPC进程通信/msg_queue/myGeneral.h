#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define getnbit(bits, begin, n) (bits >> (begin - n + 1) & ((1 << (n + 1)) - 1))
#define BCD_TO_BIN(val) ((val) = ((val)&15) + ((val) >> 4) * 10)

//处理错误信息
void handle_error(const char* msg)
{
    printf("Code location:[file %s][line %d]", __FILE__, __LINE__);
    printf("Error No.:[%d][%s] , msg : %s\n", errno, strerror(errno), msg);
    exit(EXIT_FAILURE);
}
