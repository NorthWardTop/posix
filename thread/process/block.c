/*
 * @Author: northward
 * @Github: https://github.com/northwardtop
 * @Date: 2019-02-13 19:18:28
 * @LastEditors: northward
 * @LastEditTime: 2019-02-13 20:09:13
 * @Description: 测试waitpid, 阻塞等待子进程退出
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    int pid=-1;
    perror("start test\n");
    pid=fork();
    if(pid<0)
        perror("fork failed");
    else if(pid==0) //child process
    {
        perror("I am child...");
        sleep(5);
        exit(255); //exit child process
    }
    else    //parent
    {
        int state=0;
        //将等待子进程5秒执行完毕
        pid_t ret=waitpid(0,& state, 0);
        //如果子进程真正结束, 并且返回父进程id
        if(WIFEXITED(state) && ret == pid)
            printf("wait child exited, exit code: %d, %d\n", state, WEXITSTATUS(state));
        else
            printf("wait child failed\n");
    }

    return 0;
}
