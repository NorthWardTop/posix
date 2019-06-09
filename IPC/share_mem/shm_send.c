/*
 * @Author: northward
 * @Github: https://github.com/northwardtop
 * @Date: 2019-02-09 20:56:15
 * @LastEditors: northward
 * @LastEditTime: 2019-02-12 22:32:41
 * @Description: 利用system V的信号量来控制临界资源共享内存shm
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
/* Get shared memory segment.  */
//extern int shmget (key_t __key, size_t __size, int __shmflg) __THROW;
//flg:IPC_CREAT|IPC_EXCL|SHM_W|SH_R
// shmctl(),SHM_LOCK:锁定共享内存段,SHM_UNLOCK,第二个人锁定将被阻塞
// shmat(id,0(系统指定)非0为指定地址,SHM_RDONLY|RND|),映射到用户空间,
// shmdt(取消映射的地址) == free

/**
 * @description: 主函数
 * @param {type} 
 * @return: 
 */
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
    sem_id = semget((key_t)6666, 1, IPC_CREAT | 0666);
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
        if(sem_val==0)
        {
            printf("Please enter the information to send:\n");
            fflush(stdout);
            scanf("%s", (char*)pshm);//输入内容直接写入到共享内存的指针
            sem_b.sem_op=1;//正整数加到上val
            ret=semop(sem_id, &sem_b, 1);
            if(ret<0)
                handle_error("semop failed");
            if(strcmp(pshm,"q")==0)
                break;
        }
        
    }
    ret=shmdt(pshm);
    if(ret<0)
        handle_error("detache share memory failed");
    
    
    return 0;
}
