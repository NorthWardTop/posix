#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define PATH        "."
#define PROJ_ID     2333
#define BUF_SiZE    1024

#define CLIENT_TYPE 1
#define SERVER_TYPE 2


//结构声明
struct msgbuf
{
    long mtype; //缓冲区大小
    char mtext[BUF_SIZE]; //缓冲区
}

//函数声明
