/*
 * @Author: northward
 * @Github: https://github.com/northwardtop
 * @Date: 2019-02-07 19:50:31
 * @LastEditors: northward
 * @LastEditTime: 2019-02-08 21:10:00
 * @Description: Description
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>

#define MAX_SEM 5

/**
 * @description: 主函数
 * @param {参数个数, 参数串} 
 * @return: 错误码
 */
int main(int argc, const char *argv[])
{
    int ret, sem_id;
    unsigned short sem_arr[MAX_SEM];
    unsigned short sem_read_arr[MAX_SEM];

    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *arr;
    } arg;
    //IPC_PRIVATE值为0只可以用于有亲缘关系的进程间通信, 创建信号量
    sem_id = semget(IPC_PRIVATE, MAX_SEM, IPC_CREAT | 0666);
    if (sem_id < 0)
        perror("semget err");

    for (unsigned short i = 0; i < MAX_SEM; ++i)
        sem_arr[i] = i + 1;
    arg.arr = sem_arr; //将arg的数组指针指向sem数组
    //SEMALL时候, 参数2忽略, 参数四数组中的所有信号量的值设置为
    ret=semctl(sem_id,0,SETALL,arg);
    if(ret<0)
        perror("semctl sem_arr SETALL err");
    arg.arr=sem_read_arr;
    ret=semctl(sem_id,0,SETALL,arg);
    if(ret<0)
        perror("semctl sem_read_arr SETALL err");
    //打印两个信号量数组的值
    for(int i=0;i<MAX_SEM;++i)
        printf("semaphore %d value is %d, read array is %d\n",\ 
         i, sem_arr[i], sem_read_arr[i]);
    //通过GETVAL 获得sem值并打印
    for(int i=0;i<MAX_SEM; ++i)
    {   //未指定数组
        ret=smectl(sem_id,i,GETVAL);
        printf("use GETVAL semaphore %d value is %d, read array is %d\n", \
        i, [i], sem_read_arr[i]);
    }
    //删除信号量
    ret=semctl(sem_id, 0, IPC_RMID);

    return 0;
}