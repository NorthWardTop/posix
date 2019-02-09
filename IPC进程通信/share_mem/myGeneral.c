/*
 * @Author: northward
 * @Github: https://github.com/northwardtop
 * @Date: 2019-02-09 01:38:06
 * @LastEditors: northward
 * @LastEditTime: 2019-02-09 14:57:41
 * @Description: 通用文件
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

//处理错误信息, 致命错误, 程序直接退出
void handle_error(const char* msg)
{
    char strerr[64];
    memset(strerr, '\0', sizeof(strerr));
    strcpy(strerr, strerror(errno));
    printf("No.:[%d][%s] , msg : %s\n", errno, strerr, msg);
    exit(EXIT_FAILURE);
}

//显示信息, 非致命错误, 被调程序返回错误码
void show_msg(const char* msg)
{
    printf("No.:[%d][%s] , msg : %s\n", errno, strerror(errno), msg);
}
