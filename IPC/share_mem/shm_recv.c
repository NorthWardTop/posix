/*
 * @Author: northward
 * @Github: https://github.com/northwardtop
 * @Date: 2019-02-10 12:05:29
 * @LastEditors: northward
 * @LastEditTime: 2019-02-12 22:32:29
 * @Description: 共享内存接收端, 有名信号量实现互斥
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "myGeneral.h"

int main(int argc, char const *argv[])
{
    int ret;
    int shm_id, sem_id;  //共享内存句柄和信号量句柄
    int sem_val;         //获取sem的值
    void *pshm = NULL;   //共享内存映射回来的地址
    struct sembuf sem_b; //将来操作sem时使用的参数

    //设置sembuf对象
    sem_b.sem_num = 0; //第几个sem
    // sembuf结构的sem_flg成员为SEM_UNDO时，它将使操作系统跟踪当前进程对这个信号量的修改情况，
    // 如果这个进程在没有释放该信号量的情况下终止，操作系统将自动释放该进程持有的信号量。
    // 防止其他进程因为得不到信号量而 发生【死锁现象】。
    sem_b.sem_flg = SEM_UNDO;
    //获取一个信号量,为了简单,使用系统的key,设置一个信号量

    //分配sem对象
    sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (sem_id < 0)
        handle_error("Use system V semget failed");
    //设置sem对象
    ret = semctl(sem_id, 0, SETVAL, 0);
    if (ret < 0)
    {
        handle_error("Sem value seting failed");
        ret = semctl(sem_id, 0, IPC_RMID, 0);
        if (ret < 0)
            handle_error("Sem removal failed");
    }



    //分配一段共享内存,仅仅自己读写
    shm_id = shmget((key_t)6666, (size_t)getpagesize, IPC_CREAT | 0600);
    if (shm_id < 0)
        handle_error("share memory allocation failed");
    //设置到用户空间
    pshm = shmat(shm_id, NULL, 0);
    if (pshm == NULL)
        handle_error("share memory mapping failed");

    while (1)
    {
        sem_val=semctl(sem_id, 0, GETVAL);
        if(sem_val!=0)
            continue;
        printf("get message:\n");
        fflush(stdout);
        printf("%s", (char*)pshm);//共享内存的指针读出来
        sem_b.sem_op=-1;//从1到0,直到send去+1
        ret=semop(sem_id, &sem_b, 1);
        if(ret<0)
            handle_error("semop failed");
        if(strcmp(pshm,"q")==0)
            break;
    }
    ret=shmdt(pshm);
    if(ret<0)
        handle_error("detache share memory failed");

    //销毁信号量和共享内存
    ret=shmctl(shm_id, IPC_RMID, NULL);
    if(ret<0)
        handle_error("Remove share memory failed");
    ret=semctl(sem_id, 0, IPC_RMID);
    if(ret<0)
        handle_error("Remove sem failed");
    return 0;
}
