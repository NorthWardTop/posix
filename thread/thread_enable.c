/*
 * @Author: northward
 * @Github: https://github.com/northwardtop
 * @Date: 2019-02-14 14:36:40
 * @LastEditors: northward
 * @LastEditTime: 2019-02-14 16:49:12
 * @Description: 测试线程不可取消或可取消
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


void* thread_fun(void* arg)
{
    int ret;
    //设置不可取消
    ret=pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    if(ret<0)
        perror("Thread disable cancel setting failed");
    //3秒后设置为可取消状态
    for(int i=0;i<3;++i)
    {
        printf("Threads have been set to be irrevocable and are running...\n");
        sleep(1);
    }
    //设置为可取消状态
    ret=pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    if(ret<0)
        perror("Thread enable cancel setting failed");
    printf("Thread have been set to cancelable\n");
    sleep(20); //休眠20秒, 等待取消
    printf("Automatic thread exit\n"); //执行不到这句, 主线程将取消本线程
    pthread_exit(0);
}

int main(int argc, char const *argv[])
{
    pthread_t tid;
    int ret=-1;
    //创建线程
    ret=pthread_create(&tid, NULL, thread_fun, NULL);
    if(ret!=0)
        perror("new thread create failed");
    //取消线程
    printf("canceling thread...\n");
    ret=pthread_cancel(tid);
    sleep(10);
    if(ret<0)
        perror("thread cancel failed");
    printf("thread cancel success\n");
    //等待线程结束
    ret=pthread_join(tid, NULL);
    if(ret<0)
        perror("thread join failed");
    return 0;
}

