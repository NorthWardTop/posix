/*
 * @Author: northward
 * @Github: https://github.com/northwardtop
 * @Date: 2019-02-14 21:45:29
 * @LastEditors: northward
 * @LastEditTime: 2019-02-14 22:34:42
 * @Description: 线程间的条件变量, 模拟生产者消费者两个线程同步
 *  条件变量初始化和销毁
 *      int pthread_cond_destroy(pthread_cond_t *cond);
 *      int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);
 *      pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
 *  条件满足时候通知等待条件变量的线程
 *      int pthread_cond_signal(pthread_cond_t *cond); //唤醒第一个线程
 *      int pthread_cond_broadcast(pthread_cond_t *cond); //广播唤醒所有线程
 *  等待条件变量
        //在规定时间内阻塞等待
 *      int pthread_cond_timedwait(pthread_cond_t *restrict cond, //条件变量对象吧
           pthread_mutex_t *restrict mutex, //互斥对象
           const struct timespec *restrict abstime); //超时时间
 *      //阻塞等待满足后唤醒
        int pthread_cond_wait(pthread_cond_t *restrict cond,
           pthread_mutex_t *restrict mutex);

 */

#include <pthread.h>


pthread_cond_timedwait