#pragma once

#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<errno.h>
#include<string.h>

#define _PATH_ "."
#define _PROJ_ID_ 0x6666
#define _SIZE_ 1024

#define CLIENT_TYPE 1
#define SERVER_TYPE 2

struct msgbuf
{
    long mtype;
    char mtext[_SIZE_];
};

int creat_queue();
int get_queue();

int send_msg(int msgqueue_id,int who,char *msg);
int recv_msg(int msgqueue_id,int want,char out[],int out_len);

int delete_msgqueue(int msgqueue_id);
