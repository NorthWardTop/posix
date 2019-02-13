/*
 * @Author: northward
 * @Github: https://github.com/northwardtop
 * @Date: 2019-02-13 19:18:28
 * @LastEditors: northward
 * @LastEditTime: 2019-02-13 21:12:35
 * @Description: 测试waitpid, 非阻塞等待子进程退出
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    int pid = -1;
    perror("start test\n");
    pid = fork();
    if (pid < 0)
        perror("fork failed");
    else if (pid == 0) //child process
    {
        perror("I am child...");
        printf("pid: %d", getpid());
        sleep(5);
        exit(255); //exit child process
    }
    else //parent
    {
        int state = 0;
        pid_t ret = 0;
        //将等待子进程5秒执行完毕
        do
        {
            ret = waitpid(0, &state, WNOHANG);
            if(ret == 0){
                printf("child is running\n");
            }
            sleep(1);
        }while (ret == 0);
        //如果子进程真正结束, 并且返回父进程id
        if (WIFEXITED(state) && ret == pid)
            printf("noblock wait child exited, exit code: %d, %d\n", state, WEXITSTATUS(state));
        else
            printf("wait child failed\n");
    }

    return 0;
}

/*
老师您好, 我开学就大三下学期了, 马上快要找工作或者实习了, 我希望您能帮我推荐一些就业单位:
    方向: 智能设备开发, 智能家居, 物联网车联网, 嵌入式linux, 底层开发, linux驱动开发
    技术: 潭州全套课程:Windows开发(win32/MFC较弱, 基本掌握), 
            Linux(Qt开发和Linux应用层内核层学习较深, 能直接应用)
*/