
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
//#include <sys/pool>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;


/*****************************************************************************
 函 数 名  : main
 功能描述  : 主函数
 输入参数  : int argc,char** argv
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年12月22日
    作    者   : my
    修改内容   : 新生成函数

*****************************************************************************/
int main( int argc,char** argv )
{ 
    initServer(g_pSvr);
    if(initSock(&g_pSvr->svrFd,ADDR,PORT)<0)
    {
        perror("Create server error!\n");
        exit(1);
    }
    
    
    return 0;
}













//int main( int argc,char** argv )
//{
//    int i=0;
//create project
//    g_pSvr=(Server*)malloc(sizeof(Server));
//    if(!g_pSvr)
//    {
//        perror("Create server error!\n");
//        exit(1);
//    }

//initial or seting project
//    memset(g_pSvr,NULL,sizeof(Server));
//    for(i=0;i<MAX_FD_SIZE;++i)//All FDS are initialized to -1 
//        g_pSvr->cltFds[i]=-1;
//    //create global socket
//    g_pSvr->svrFd=socket(AF_INET,SOCK_STREAM,0);
//    if(-1==g_pSvr->svrFd)
//    {
//        perror("create server socket error!\n");
//        exit(1);
//    }
//    //initial address information
//    g_pSvr->svrAddr.sin_family = AF_INET;
//    g_pSvr->svrAddr.sin_port = htons(PORT);
//    g_pSvr->svrAddr.sin_addr.s_addr = htonl(INADDR_ANY);//Allow any address

//    //bind addr info with global socket
//    if(-1==bind(g_pSvr->svrFd, \
//        (struct sockaddr*)&g_pSvr->svrAddr,sizeof(g_pSvr->svrAddr)))
//    {
//        perror("server socket bind error!\n");
//        exit(1);
//    }
//    //listen server Fd
//    if(-1==listen(g_pSvr->svrFd,MAX_FD_SIZE))
//    {
//        perror("server socket listen error!\n");
//        exit(1);
//    }

//    while(1)
//    {
//        
//    }
//}

//==============================
//    //point bind all FdSet
//    fd_set *readFdSet = &g_pSvr->allFdSet;

//    while(1)
//    {
//        int ret;
//        FD_ZERO(readFdSet);//init the fd box
//        FD_SET(g_pSvr->svrFd,readFdSet);//global fd put in the read box
//        g_pSvr->maxFds=g_pSvr->svrFd;

//        for(i=0;i<MAX_FD_SIZE;++i)
//        {//FD_SET in all fd and find max fd assign with maxFds
//            int tmpFd = g_pSvr->cltFds[i];
//            FD_SET(tmpFd,readFdSet);
//            g_pSvr->maxFds = tmpFd > g_pSvr->maxFds? \
//                tmpFd:g_pSvr->maxFds;
//        }
//    
//        //seting timeout 
//        g_pSvr->tv.tv_sec=20;
//        g_pSvr->tv.tv_usec=0;

//        //select loop
//        ret=select(g_pSvr->maxFds+1,readFdSet,NULL,NULL,&g_pSvr->tv);
//          
//        if(-1==ret)//error return
//        {
//            perror("select error!\n");
//            exit(1);
//        }
//        else if(ret==0)//timeout return 
//        {
//            printf("select timeout!\n");
//            continue;
//        }
//        //have sock return,is listen fd?
//        if(FD_ISSET(g_pSvr->svrFd,readFdSet))
//        {
//            //listen fd;
//            struct sockaddr_in newClt;
//            socklen_t len=sizeof(newClt);
//            int conFd=-1;
//            while(-1==conFd)
//            {
//                conFd=accept(g_pSvr->svrFd, \
//                    (struct sockaddr*)&g_pSvr->svrAddr,&len);
//                if(-1==conFd)
//                    perror("accept error!\n");
//            }
//            //put in cltFds[]
//            for(i=0;i<MAX_FD_SIZE;++i)
//            {
//                if(g_pSvr->cltFds[i]<0)
//                {//find empty place
//                    g_pSvr->cltFds[i]=conFd;
//                    g_pSvr->currCltCnt++;//bug over MAX_FD
//                    break;
//                }
//            }
//        }
//        //在已连接的客户端box中找有数据的fd
//        char rBuf[1024];
//        for(i=0;i< g_pSvr->currCltCnt;++i)
//        {
//            int tmpFd = g_pSvr->cltFds[i];
//            if(tmpFd<0)
//                continue;
//            //curr fd have data?
//            if(FD_ISSET(tmpFd,readFdSet))
//            {
//                int readRet=read(tmpFd,rBuf,sizeof(rBuf));
//                if(readRet<=0)//read error
//                {    
//                    FD_CLR(tmpFd,&g_pSvr->allFdSet);
//                    close(tmpFd);
//                    g_pSvr->cltFds[i]=-1;
//                    continue;
//                }
//                //if readed then print
//                printf("%s\n",rBuf);
//            }
//	        string wBuf="hello ,clt";
//           // char wBuf[1024]="hello form server";
//            write(tmpFd,wBuf.c_str(),wBuf.length()+1);
//        }
//    }
//    
//    free(g_pSvr);
//    return 0;
//}





































