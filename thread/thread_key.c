/*
 * @Author: northward
 * @Github: https://github.com/northwardtop
 * @Date: 2019-02-14 19:37:41
 * @LastEditors: northward
 * @LastEditTime: 2019-02-14 19:38:01
 * @Description: 测试pthread_key_create()
 * int pthread_key_create (pthread_key_t *__key, void (*__destr_function) (void *))
 *      创建的线程具有私有数据key
 *      不同线程可以使用同名的数据, 但是具有不同的地址和空间
 *      从TSD池分配, 注销(pthread_key_delete(key))返回到池, 
 *      注销不会检查其他线程是否使用中, 并将数据置NULL
 *      线程退出时候调用参数二函数指针
 *      类似于虚函数, 同名但不同类实现方法不同
 * 读写私有数据
 *      int pthread_setspecific 
 *      void* pthread_getspecific 
 *      
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int main(int argc, char const *argv[])
{
    pthread_key_t key;

    

    return 0;
}
