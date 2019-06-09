#include"common.h"

int server()
{
    int msg_id = creat_queue();
    if(msg_id < 0){
        printf("%s\n",strerror(errno));
        return -1;
    }
    char buf[_SIZE_];
    while(1)
    {
        sleep(5);
        memset(buf,'\0',sizeof(buf));
        int ret = recv_msg(msg_id,CLIENT_TYPE,buf,sizeof(buf));
        if(ret == 0){
            if(strncasecmp(buf,"quit",4) == 0){
                printf("client leave!\n");
                break;
            }
            printf("client say : %s\n",buf);
        }
        printf("Please Enter :");
        fflush(stdout);
        memset(buf,'\0',sizeof(buf));
        scanf("%s",buf);
        send_msg(msg_id,SERVER_TYPE,buf);
    }
    return delete_queue(msg_id);
}
int main()
{
    server();
    return 0;
}
