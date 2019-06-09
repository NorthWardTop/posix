/*
 * @Author: northward
 * @Github: https://github.com/northwardtop
 * @Date: 2019-02-01 21:17:17
 * @LastEditors: northward
 * @LastEditTime: 2019-02-02 19:36:17
 * @Description: Description
 */


#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "myGeneral.h"

#define PATH        "."
#define PROJ_ID     2333
#define BUF_SIZE    1024

#define CLIENT_TYPE 1
#define SERVER_TYPE 2


//结构声明
struct msgbuf
{
    long mtype; //缓冲区大小
    char mtext[BUF_SIZE]; //缓冲区
};

//函数声明
int creat_queue(int flags);
int get_queue();
int delete_queue(int msg_id);
int recv_msg(int msg_id, int type, char* buf, int buf_size);
ssize_t send_msg(int msg_id, int type, const char* msg);
