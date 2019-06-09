/*
 * @Author: northward
 * @Github: https://github.com/northwardtop
 * @Date: 2019-02-08 22:23:32
 * @LastEditors: northward
 * @LastEditTime: 2019-02-09 16:03:37
 * @Description: 测试线程间无名信号量的通信
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>


sem_t sem; //线程间共享的信号量

/**
 * @description: 第一个线程主函数
 * @param {泛参} 
 * @return: 泛返回
 */
void* thread1(void* arg)
{
    sem_wait(&sem);//P
    printf("Before 1st thread changed: %d\n", *(int*)arg);
    (*(int*)arg)++;
    printf("After 1st thread changed: %d\n", *(int*)arg);
    sem_post(&sem);//V
}

/**
 * @description: 第二个线程主函数
 * @param {type} 
 * @return: 
 */
void* thread2(void* arg)
{
    sem_wait(&sem);//P
    printf("Before 2th thread changed: %d\n", *(int*)arg);
    (*(int*)arg)--;
    printf("After 2th thread changed: %d\n", *(int*)arg);
    sem_post(&sem);//V
}


/**
 * @description: 无名用于相关进程或线程间的同步
 * @param {type} 
 * @return: 
 */
int main(int argc, char const *argv[])
{
    int num=1; 
    pthread_t tid1,tid2;
    //给sem初始化信号量, 线程间共享, 初值为1(多少个资源/可P多少次)
    sem_init(&sem, 0, 1);
    //attr参数指向一个pthread_attr_t结构，其内容用于
//         线程创建时间，以确定新线程的属性; 这个结构
//         使用pthread_attr_init（3）和相关函数初始化。 如果attr是
//         NULL，然后使用默认属性创建线程。
    pthread_create(&tid1,NULL,thread1, &num);
    pthread_create(&tid2,NULL,thread2, &num);
    pthread_join(tid1, NULL);//启动并等待线程1结束,NULL输出为线程的返回值void*
    pthread_join(tid2, NULL);
    return 0;
}
