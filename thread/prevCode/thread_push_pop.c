/*
 * @Author: northward
 * @Github: https://github.com/northwardtop
 * @Date: 2019-02-14 17:23:01
 * @LastEditors: northward
 * @LastEditTime: 2019-02-14 19:23:57
 * @Description: 测试线程启动时压入清理函数, 
 * 在线程退出时候弹出清理函数, 类似于析构函数
 * 结果, 没有太整明白怎么释放资源的
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <list.h>

/**
 * @description: 线程清理函数, 类似于析构函数
 * @param {要清理的对象} 
 * @return: 
 */
void cleanup(int* _p)
{
    printf("cleaning thread resources\n");
    if(_p!=NULL)
    {
        printf("free resource");
        free(_p);
    }
    _p=NULL;
}

/**
 * @description: 线程执行函数
 * @param {type} 
 * @return: 
 */
void* thread_fun(void* arg)
{
    printf("thread_fun start\n");
    int* p=(int*)malloc(sizeof(int));
    //压入线程清理函数
    pthread_cleanup_push(cleanup, p);
    pthread_cleanup_pop(1);
    printf("thread end\n");
}


int main(int argc, char const *argv[])
{
    pthread_t tid;
    pthread_create(&tid, NULL, thread_fun, NULL);
    sleep(1);
    pthread_join(tid, NULL);

    return 0;
}
