#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "myGeneral.h"

#define ADDR "127.0.0.1"
#define PORT 80
#define BUF_SIZE 1024
#define REQUEST_SIZE 4096

int main(int argc, const char **argv)
{

    //连接服务器部分------------------
    int ret, sock_fd;
    struct sockaddr_in srv_addr;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
        handle_error("create socket err");

    //填充sockaddr对象
    bzero(&srv_addr, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(PORT);
    ret = inet_pton(AF_INET, ADDR, &srv_addr.sin_addr);
    // if (ret < 0)
    //     handle_error("string to binary err");
    if (ret <= 0)
    {
        if (ret == 0)
            fprintf(stderr, "Not in presentation format");
        else
            perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    ret = connect(sock_fd, (struct sockaddr *)&srv_addr, sizeof(struct sockaddr));
    if (ret < 0)
    {
        close(sock_fd);
        handle_error("connect err");
    }
    printf("连接服务器成功\n");

    //拼接请求部分------------------------
    char requer[REQUEST_SIZE];
    char qqCode[REQUEST_SIZE];
    char str_qqCodeLen[128];
    //置空QQCode请求参数
    memset(qqCode, 0, REQUEST_SIZE);
    //设置QQCode请求参数
    strcat(qqCode, "qqCode=474497857");
    //获取qqCode的长度, 请求参数转化为字符串
    sprintf(str_qqCodeLen, "%d", (int)strlen(qqCode));

    //置空请求串
    memset(requer, 0, REQUEST_SIZE);
    //连请求行
    strcat(requer, "POST /webservices/qqOnlineWebService.asmx/qqCheckOnline HTTP/1.1\n");
    //服务器的DNS名称。从URL中提取出来，必需。
    strcat(requer, "Host: www.webxml.com.cn\n");
    //页面的MIME类型
    strcat(requer, "Content-Type: application/x-www-form-urlencoded\n");
    //以字节计算的页面长度
    strcat(requer, "Content-Length: ");
    strcat(requer, str_qqCodeLen);
    strcat(requer, "\n\n");
    //请求参数qqCode
    strcat(requer, qqCode);
    strcat(requer, "\r\n\r\n");
    printf("requer: %s\n", requer);
    //写入socket, 发出请求
    ret = write(sock_fd, requer, strlen(requer));
    if (ret < 0)
    {
        close(sock_fd);
        handle_error("request");
    }
    else
        printf("Successful sending of requests: %d bytes", ret);

    //-----------select部分------------------------
    fd_set set_fds;
    struct timeval tv;
    FD_ZERO(&set_fds);
    FD_SET(sock_fd, &set_fds);
    char buf[BUF_SIZE];
    int read_size;

    while (1)
    {
        sleep(2);
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        ret = select(sock_fd + 1, &set_fds, NULL, NULL, &tv);
        if (ret < 0)
        {
            close(sock_fd);
            handle_error("select set_fds err");
        }
        else if (ret > 0)
        {
            memset(buf, 0, BUF_SIZE);
            read_size = read(sock_fd, buf, BUF_SIZE);
            if (read_size == 0)
            {
                close(sock_fd);
                handle_error("Read socket data err, The other party is closed!");
            }
            printf("socket say: %s", buf);
        }
    }
    close(sock_fd);
    return 0;
}