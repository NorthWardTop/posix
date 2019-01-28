#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "myGeneral.h"


#define ADDR            "127.0.0.1"
#define PORT            80
#define BUF_SIZE        1024
#define REQUEST_SIZE    4096

int main(int argc, const char** argv)
{
    int ret, sock_fd;
    struct sockaddr_in srv_addr;


    sock_fd=socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd<0)
        handle_error("create socket err");
    
    //填充sockaddr对象
    bzero(&srv_addr, sizeof(srv_addr));
    srv_addr.family=AF_INET;
    srv_addr.sin_port=htons(PORT);
    ret=inet_pton(AF_INET, ADDR, &srv_addr.sin_addr);
    if(ret<0)
        handle_error("string to binary err");

    ret=connect(sock_fd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
    if(ret<0)
        handle_error("connect err");
    
    printf("连接服务器成功\n");
    
    //发送数据
    char buf[BUF_SIZE];
    char requer[REQUEST_SIZE];
    char qqCode[REQUEST_SIZE];
    char str_qqCodeLen[128];
    //置空QQCode请求参数
    memset(qqCode, 0, REQUEST_SIZE);
    //设置QQCode请求参数
    strcat(qqCode, "qqCode=474497857");
    //获取qqCode的长度, 请求参数转化为字符串
    sprintf(str_qqCodeLen, "%d", strlen(qqCode));

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
    ret=write(sock_fd,requer, strlen(requer));
    if(ret<0)
        handle_error("request")
}