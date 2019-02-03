/*
 * @Author: northward
 * @Github: https://github.com/northwardtop
 * @Date: 2019-02-01 21:26:10
 * @LastEditors: northward
 * @LastEditTime: 2019-02-02 18:55:43
 * @Description: Description
 */

#include "common.h"
#include "myGeneral.h"

/**
 * @description: 主函数
 * @param {程序参数个数, 参数字符串} 
 * @return: 错误码
 */
int main(int argc, const char** argv)
{
    int ret, msg_id;
    char buf[BUF_SIZE];
    msg_id=get_queue(); //调用creat_queue, 将找到和server创建id相同的msg队列

    while(strcmp(buf, "q")==0)
    {
        printf("Please enter the message to be sent:\n");
        fflush(stdout);
        memset(buf, '\0', sizeof(buf));
        scanf("%s", buf);
        ret=send_msg(msg_id, CLIENT_TYPE, buf);
        if(ret<0)
            show_msg("client send failed\n");

        memset(buf, '\0', sizeof(buf));
        ret = recv_msg(msg_id, CLIENT_TYPE, buf, sizeof(buf));
        if(ret<0)
            show_msg("client receive failed\n");
    }
}

