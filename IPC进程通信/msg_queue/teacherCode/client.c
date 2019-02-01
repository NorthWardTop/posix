#include"common.h"

int client()
{
    int msg_id = get_queue();
    char buf[_SIZE_];
    while(1)
    {
        printf("Please Enter : ");
        fflush(stdout);
        memset(buf,'\0',sizeof(buf));
        scanf("%s",buf);
        send_msg(msg_id,CLIENT_TYPE,buf);
        if(strncasecmp(buf,"quit",4) == 0){
            printf("client quit\n");
            return 0;
        }
        sleep(2);
        memset(buf,'\0',sizeof(buf));
        int ret = recv_msg(msg_id,SERVER_TYPE,buf,sizeof(buf));
        if(ret == 0){
            printf("server say : %s\n",buf);
        }
    }
}
int main()
{
    client();
    return 0;
}
