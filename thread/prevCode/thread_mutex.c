/*
 * @Author: northward
 * @Github: https://github.com/northwardtop
 * @Date: 2019-02-14 20:22:36
 * @LastEditors: northward
 * @LastEditTime: 2019-02-14 20:23:55
 * @Description: 线程同步互斥锁机制测试
 * 开锁0, 上锁1, 自己上锁自己解
 * 初始化锁
 *  int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
 *  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //直接赋值初始化
 *  int pthread_mutex_destroy(pthread_mutex_t *mutex);
 * 上锁解锁
 *  int pthread_mutex_lock(pthread_mutex_t *mutex); //阻塞
 *  int pthread_mutex_trylock(pthread_mutex_t *mutex); //非阻塞,没申请到立即返回错误
 *  int pthread_mutex_unlock(pthread_mutex_t *mutex); //解锁(谁lock的,谁去unlock)
 * 坑:
 *  终极bug:线程a:i++ j-- 线程b:i==j
 *  解决方案:条件变量 pthread_cond_t
 *  
 * 
 *  
 */




