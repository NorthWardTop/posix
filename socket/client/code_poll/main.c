#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "init.h"







/*****************************************************************************
 函 数 名  : main
 功能描述  : 主函数
 输入参数  : int argc
             char *argv[]
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月26日
    作    者   : chandler
    修改内容   : 新生成函数

*****************************************************************************/
int main( int argc, char *argv[] )
{
	int iListenFd,iConnFd;
	int iRet;
	iRet=InitServer();
	if(iRet<0)
	{
	  perror("InitServer ERR\n");
	  return -iRet;
	}
	g_ptServer->listenfd=InitSocket(IPADDRESS,PORT);
	listen(g_ptServer->listenfd,LISTENNUM);
	DoPollHandle(g_ptServer->listenfd);
    return 0;
}





















