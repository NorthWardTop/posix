#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "myGeneral.h"

int main()
{
    int pipe_fd[2] = {-1, -1};
    int ret = pipe(pipe_fd);
    if (ret < 0)
        handle_error("pipe err");
    //1 --> 0 写到读 子到父
    pid_t pid = fork();
    if (pid == 0)
    {
        //子进程关闭读, 只去写
        close(pipe_fd[0]);
        char *msg = "child write: I am a child process";
        while (1)
        {
            ret = write(pipe_fd[1], msg, strlen(msg));
            sleep(1);
        }
    }
    else
    {
        //父进程关闭写, 只去写
        close(pipe_fd[1]);
        char read_buf[128];
        while (1)
        {
            ssize_t _recv_sz = read(pipe_fd[0], read_buf, 128);
            // if (_recv_sz > 0)
            //     read_buf[_recv_sz] = '\0';
            printf("father read: %ld %s\n",_recv_sz, read_buf);
        }
        wait(NULL);//等待0进程(子进程退出)
    }
    return 0;
}
