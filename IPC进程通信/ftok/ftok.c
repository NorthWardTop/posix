#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "myGeneral.h"

// key_t ftok(const char *pathname, int proj_id);
// 	const char *pathname  一般设定为当前目录 .  必须存在
// 	/test/ ----
// 	stat(/test/)------struct stat
// 	stat.st_dev
// 	stat.ino
// 	key  31-24 为proj_id的低8位
// 		 23-16 为st_dev的低8位
// 		 15-0  为ino的低16位

int main(int argc, char **argv)
{
    key_t k;
    struct stat file_stat;
    int ret, num;
    if (argc != 3)
    {
        printf("Usage: %s path number", argv[0]);
        exit(0);
    }
    //ret=access(".", F_OK);
    ret = stat(argv[1], &file_stat);
    if (ret < 0)
        handle_error("file must already exist");
    num = atoi(argv[2]); //ascii to int
    printf("proj_id %x\n", num);
    printf("file st_dev %x\n", file_stat.st_dev);
    printf("file st_ino %x\n", file_stat.st_ino);
    k = ftok(argv[1], num);

    printf("k>>24=%x, (k>>16)<<8=%x, k<<16=%x\n", k >> 24, (k >> 16) << 8, k << 16);

    return 0;
}

// 设x为6位(二进制24位)16进制数变量，lx为低两位数变量，mx为中两位数变量，hx高两位数变量，则：
// hx=x>>16&0xFF; 高8 23-16 23,8
// mx=x>>8&0xFF;  中8 15-8  15,8
// lx=x>>0&0xFF;  低8 7-0   7,8
// getnbit(bits, begin, n)  num>>(begin-n+1) & (1 << n)

// n -> 111l
// 4 -> 1111
// 1 << n