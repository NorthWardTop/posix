/*
 * @Author: northward
 * @Github: https://github.com/northwardtop
 * @Date: 2019-02-01 21:26:19
 * @LastEditors: northward
 * @LastEditTime: 2019-02-02 19:39:29
 * @Description: Description
 */

#include "common.h"
#include "myGeneral.h"


int main(int argc, const char **argv)
{
    //服务端(创建端)消息队列使用方法
    /*
1. 创建消息队列, 返回msg_id
    1. 使用ftok传入路径和随机数值, 返回key_t
    2. 使用msgget传入key_t和flags(IPC_CREAT | IPC_EXCL | 权限), 返回msg_id
2. while(1)
    1.接受消息, 如果有消息就打印, 没有消息就等待输入和发送
3. 销毁队列
*/
    int ret;
    char buf[BUF_SIZE];
    int msg_id = creat_queue(IPC_CREAT | IPC_EXCL | 0666);
    if (msg_id < 0)
        handle_error("create message queue err");
    while (strcmp(buf, "q") == 0)
    {
        memset(buf, '\0', BUF_SIZE);
        ret = recv_msg(msg_id, CLIENT_TYPE, buf, sizeof(buf));
        if (ret < 0)
            handle_error("receive err from message queue");
        printf("client say: %s\n", buf);

        printf("Please enter the message to be sent:\n");
        // fflush(stdout);
        scanf("%s", buf);
        ret = send_msg(msg_id, SERVER_TYPE, buf);
        if (ret < 0)
            printf("send failed");
    }
    ret = delete_queue(msg_id);
    if (ret < 0)
        handle_error("delete message queue failed");

    return 0;
}