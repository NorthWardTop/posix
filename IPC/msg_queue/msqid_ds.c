/*
 * @Author: northward
 * @Github: https://github.com/northwardtop
 * @Date: 2019-01-31 21:12:28
 * @LastEditors: northward
 * @LastEditTime: 2019-02-02 16:49:43
 * @Description: Description
 */

#include "common.h"
#include "myGeneral.h"



/**
 * @description: 根据提供的flags生成消息队列, 返回队列id
 * @param {flags可以包括IPC_CREAT | IPC_EXCL | 和权限eg.0777} 
 * @return: 
 */
int creat_queue(int flags)
{
	//生成key
	key_t key=ftok(PATH, PROJ_ID);
	if(key<0)
		handle_error("failed to generate key");
	int msg_id=msgget(key, flags);
	if(msg_id<0)
		handle_error("msgget failed");
	return msg_id;
}

/**
 * @description: 获取一个已存在的队列
 * @param {type} 
 * @return: 返回队列id
 */
int get_queue()
{
	//获取一个已存在的队列flg参数不需要IPC_EXCL | 0666
	return creat_queue(IPC_CREAT);
}

/**
 * @description: 根据队列id, 销毁消息队列
 * @param {队列id} 
 * @return: 错误码
 */
int delete_queue(int msg_id)
{
	int ret=msgctl(msg_id, IPC_RMID, NULL);
	if(ret<0)
		handle_error("msgctl-remove message queue failed");
	return 0;
}

/**
 * @description: 接受消息
 * @param {队列id, 接受类型} 
 * type=0,接受消息队列头个消息
 * type>0,队列中type类型的第一条消息除非在msgflg中指定了msg_except，
 * 			否则将被读取，在这种情况下，第一个将读取类型不等于msgtyp的队列中的消息。
 * type<0,队列中的第一条消息将读取小于或等于type绝对值的类型。
 * @return: 错误码
 */
int recv_msg(int msg_id, int type, char* buf, int buf_size)
{
	struct msgbuf _buf;
	memset(&_buf, '\0', sizeof(_buf));
	//消息保存进_buf结构
	int ret=msgrcv(msg_id, &_buf, sizeof(_buf.mtext), type, 0); 
	if(ret<0)
	{
		show_msg("msgrcv failed");
		return -errno;
	}
	memset(buf, '\0', buf_size);
	strcpy(buf, _buf.mtext); //返回到buf指针
	return 0;
}

/**
 * @description: 发送消息
 * @param {队列id, 发送类型, 消息指针} 
 * @return: 发送字节大小, 错误返回错误码
 */
ssize_t send_msg(int msg_id, int type, const char* msg)
{
	int ret;
	struct msgbuf _buf;

	memset(&_buf, '\0', sizeof(_buf));
	_buf.mtype=type;
	strncpy(_buf.mtext, msg, sizeof(msg)+1);

	ret=msgsnd(msg_id, &_buf, sizeof(_buf.mtext),0);
	if(ret<0)
	{
		show_msg("msgsnd failed");
		return -errno;
	}
	printf("send message: %s\n", _buf.mtext);
	return 0;
}











// struct msqid_ds {
// 	struct ipc_perm msg_perm;
// 	struct msg *msg_first;		/* first message on queue,unused  */
// 	struct msg *msg_last;		/* last message in queue,unused */
// 	__kernel_time_t msg_stime;	/* last msgsnd time */
// 	__kernel_time_t msg_rtime;	/* last msgrcv time */
// 	__kernel_time_t msg_ctime;	/* last change time */
// 	unsigned long  msg_lcbytes;	/* Reuse junk fields for 32 bit */
// 	unsigned long  msg_lqbytes;	/* ditto */
// 	unsigned short msg_cbytes;	/* current number of bytes on queue */
// 	unsigned short msg_qnum;	/* number of messages in queue */
// 	unsigned short msg_qbytes;	/* max number of bytes on queue */
// 	__kernel_ipc_pid_t msg_lspid;	/* pid of last msgsnd */
// 	__kernel_ipc_pid_t msg_lrpid;	/* last receive pid */
// };









