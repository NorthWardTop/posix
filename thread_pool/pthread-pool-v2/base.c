#include "pthread_pool.h"

/*
 *child_work:the code exec in child thread
 *ptr: the ds of thread_node of current thread.
 *return :nothing.void * just avoid warning.
 */
void *child_work(void *ptr)
{
	//获取到线程节点
	THREAD_NODE *self = (THREAD_NODE *)ptr;

	//将要修改本线程节点, 上锁线程
	/*modify the tid attribute the first time exec */
	pthread_mutex_lock(&self->mutex);
	//可有可无
	self->tid = syscall(SYS_gettid);

	pthread_mutex_unlock(&self->mutex);
	//重复的去执行任务或者阻塞等待任务
	while (1)
	{
		//修改阻塞等待前上锁
		pthread_mutex_lock(&self->mutex);
		//如果没有任务, 则阻塞
		/*if no task exec,blocked */
		if (NULL == self->work)
		{//没有任务的线程将在这里停止/休眠
			pthread_cond_wait(&self->cond, &self->mutex);
		}
		//线程已经处于唤醒状态
		//如果没有被停止, 则说明有任务, 首先上锁任务, 防止其他线程抢任务
		pthread_mutex_lock(&self->work->mutex);
		//执行这个任务
		/*execute the real work. */
		self->work->fun(self->work->arg);
		//完成任务后清理任务
		/*after finished the work */
		self->work->fun = NULL;//回去睡眠
		self->work->flag = 0;
		self->work->tid = 0;
		self->work->next = NULL;
		//按顺序释放线程参数空间,最好使用pthread_clean
		free(self->work->arg);
		//解锁和销毁任务锁
		pthread_mutex_unlock(&self->work->mutex); // unlock the task
		pthread_mutex_destroy(&self->work->mutex);

		/*free the task space */
		free(self->work);
		//使得工作函数指针为NULL
		/*make self thread no work */
		self->work = NULL;
		self->flag = 0;
		//获取工作队列的工作前上锁队列
		pthread_mutex_lock(&task_queue_head->mutex);

		/*
		 *get new task from the task_link if not NULL.
		 *there no idle thread if there are task to do.
		 *if on task ,make self idle and add to the idle queue.
		 */
		if (task_queue_head->head != NULL)
		{
			//拿到头结点指针准备去执行
			TASK_NODE *temp = task_queue_head->head;

			/*get the first task *///头结点后移
			task_queue_head->head = task_queue_head->head->next;
			//修改属性, 使线程节点处理运行中
			/*modify self thread attribute */
			self->flag = 1;
			self->work = temp;//任务
			temp->tid = self->tid;//任务执行线程的id也是线程自己的id
			temp->next = NULL;//不使用的指针置空
			temp->flag = 1;//任务已经派发
			
			task_queue_head->number--;
			//任务队列操作完成, 解锁
			pthread_mutex_unlock(&task_queue_head->mutex);
			//任务给线程派发完成,解锁线程??
			pthread_mutex_unlock(&self->mutex);

			continue;
		}
		else
		{
			/*no task need to exec, add self to idle queue and del from busy queue */
			pthread_mutex_unlock(&task_queue_head->mutex);

			pthread_mutex_lock(&pthread_queue_busy->mutex);

			/*self is the last execte thread */
			if (pthread_queue_busy->head == self &&
				pthread_queue_busy->rear == self)
			{
				pthread_queue_busy->head = pthread_queue_busy->rear = NULL;
				self->next = self->prev = NULL;
			}

			/*the first one thread in busy queue */
			else if (pthread_queue_busy->head == self &&
					 pthread_queue_busy->rear != self)
			{
				pthread_queue_busy->head = pthread_queue_busy->head->next;
				pthread_queue_busy->head->prev = NULL;

				self->next = self->prev = NULL;
			}

			/*the last one thread in busy queue */
			else if (pthread_queue_busy->head != self &&
					 pthread_queue_busy->rear == self)
			{
				pthread_queue_busy->rear = pthread_queue_busy->rear->prev;
				pthread_queue_busy->rear->next = NULL;

				self->next = self->prev = NULL;
			}

			/*middle one */
			else
			{
				self->next->prev = self->prev;
				self->prev->next = self->next;
				self->next = self->prev = NULL;
			}

			pthread_mutex_unlock(&pthread_queue_busy->mutex);

			/*add self to the idle queue */
			pthread_mutex_lock(&pthread_queue_idle->mutex);

			/*now the idle queue is empty */
			if (pthread_queue_idle->head == NULL ||
				pthread_queue_idle->head == NULL)
			{
				pthread_queue_idle->head = pthread_queue_idle->rear = self;
				self->next = self->prev = NULL;
			}

			else
			{
				self->next = pthread_queue_idle->head;
				self->prev = NULL;
				self->next->prev = self;

				pthread_queue_idle->head = self;
				pthread_queue_idle->number++;
			}

			pthread_mutex_unlock(&pthread_queue_idle->mutex);

			pthread_mutex_unlock(&self->mutex);

			/*signal have idle thread */
			pthread_cond_signal(&pthread_queue_idle->cond);
		}
	}
}

/*
 *create thread pool when the system on, and thread number is THREAD_DEF_NUM.
 *when init, initial all the thread into a double link queue and all wait fo
 *self->cond.
 */
//分配空闲线程池每个线程对象
void create_pthread_pool(void)
{
	THREAD_NODE *temp =
		(THREAD_NODE *)malloc(sizeof(THREAD_NODE) * THREAD_DEF_NUM);

	if (temp == NULL)
	{
		printf(" malloc failure\n");
		exit(EXIT_FAILURE);
	}

	/*init as a double link queue */
	int i;
	//设置对象
	for (i = 0; i < THREAD_DEF_NUM; i++)
	{
		temp[i].tid = i + 1;
		temp[i].work = NULL;
		temp[i].flag = 0;

		if (i == THREAD_DEF_NUM - 1)
			temp[i].next = NULL;

		if (i == 0)
			temp[i].prev = NULL;

		temp[i].prev = &temp[i - 1];
		temp[i].next = &temp[i + 1];

		pthread_cond_init(&temp[i].cond, NULL);
		pthread_mutex_init(&temp[i].mutex, NULL);

		/*create this thread */
		pthread_create(&temp[i].tid, NULL, child_work, (void *)&temp[i]);
	}

	/*modify the idle thread queue attribute */
	pthread_mutex_lock(&pthread_queue_idle->mutex);

	pthread_queue_idle->number = THREAD_DEF_NUM;
	pthread_queue_idle->head = &temp[0];
	pthread_queue_idle->rear = &temp[THREAD_DEF_NUM - 1];

	pthread_mutex_unlock(&pthread_queue_idle->mutex);
}

/*
 *init_system :init the system glob pointor.
 */
//对各个队列和队列条件变量和互斥锁的初始化
void init_system(void)
{
	/*init the pthread_queue_idle */
	pthread_queue_idle = (PTHREAD_QUEUE_T *)malloc(sizeof(PTHREAD_QUEUE_T));

	pthread_queue_idle->number = 0;
	pthread_queue_idle->head = NULL;
	pthread_queue_idle->rear = NULL;
	pthread_mutex_init(&pthread_queue_idle->mutex, NULL);
	pthread_cond_init(&pthread_queue_idle->cond, NULL);

	/*init the pthread_queue_busy */
	pthread_queue_busy = (PTHREAD_QUEUE_T *)malloc(sizeof(PTHREAD_QUEUE_T));

	pthread_queue_busy->number = 0;
	pthread_queue_busy->head = NULL;
	pthread_queue_busy->rear = NULL;
	pthread_mutex_init(&pthread_queue_busy->mutex, NULL);
	pthread_cond_init(&pthread_queue_busy->cond, NULL);

	/*init the task_queue_head */
	task_queue_head = (TASK_QUEUE_T *)malloc(sizeof(TASK_QUEUE_T));

	task_queue_head->head = NULL;
	task_queue_head->number = 0;
	pthread_cond_init(&task_queue_head->cond, NULL);
	pthread_mutex_init(&task_queue_head->mutex, NULL);

	/*create thread poll */
	create_pthread_pool();
}

/*
 *thread_manager:code exec in manager thread.
 *               block on task_queue_head->cond when no task come.
 *               block on pthread_queue_idle->cond when no idle thread
 *ptr:no used ,in order to avoid warning.
 *return :nothing.
 */
//
void *thread_manager(void *ptr)
{
	while (1)
	{
		THREAD_NODE *temp_thread = NULL;
		TASK_NODE *temp_task = NULL;

		/*
		 *get a new task, and modify the task_queue.
		 *if no task block om task_queue_head->cond.
		 */
		//锁定任务队列,等待
		pthread_mutex_lock(&task_queue_head->mutex);
		//如果任务队列没有任务就条件等待(管理线程阻塞)
		if (task_queue_head->number == 0)
			pthread_cond_wait(&task_queue_head->cond, &task_queue_head->mutex);
		//如果不阻塞就有任务,获得任务队列第一个任务
		temp_task = task_queue_head->head;
		task_queue_head->head = task_queue_head->head->next;
		task_queue_head->number--;
		//取完任务后解锁队列
		pthread_mutex_unlock(&task_queue_head->mutex);

		/*
		 *get a new idle thread, and modify the idle_queue.
		 *if no idle thread, block on pthread_queue_idle->cond.
		 */
		//锁定空闲线程队列
		pthread_mutex_lock(&pthread_queue_idle->mutex);
		//没有空闲队列管理线程就阻塞
		if (pthread_queue_idle->number == 0)
			pthread_cond_wait(&pthread_queue_idle->cond, &pthread_queue_idle->mutex);
		//如果不阻塞管理线程不阻塞,说明空闲队列线程有线程,取出第一个线程
		temp_thread = pthread_queue_idle->head;
		//如果使用的是最后一个空闲线程,置空队列指针
		/*if this is the last idle thread ,modiry the head and rear pointor */
		if (pthread_queue_idle->head == pthread_queue_idle->rear)
		{
			pthread_queue_idle->head = NULL;
			pthread_queue_idle->rear = NULL;
		}
		/*if idle thread number>2, get the first one,modify the head pointor  */
		//否则就后移,如果最后一个节点后移,将访问到空指针
		else
		{
			pthread_queue_idle->head = pthread_queue_idle->head->next;
			pthread_queue_idle->head->prev = NULL; //head变化后向前的指针置空, NULL<-2
		}

		pthread_queue_idle->number--;
		//空闲线程队列解锁
		pthread_mutex_unlock(&pthread_queue_idle->mutex);
		//以上已经获得线程,开始从修改任务属性,占用本任务
		/*modify the  task attribute. */
		pthread_mutex_lock(&temp_task->mutex);

		temp_task->tid = temp_thread->tid;
		temp_task->next = NULL;
		temp_task->flag = 1;

		pthread_mutex_unlock(&temp_task->mutex);
		//以上线程和任务都已经就绪,最后一步将任务和线程结合
		/*modify the idle thread attribute. */
		pthread_mutex_lock(&temp_thread->mutex);

		temp_thread->flag = 1;
		temp_thread->work = temp_task;
		temp_thread->next = NULL;
		temp_thread->prev = NULL;

		pthread_mutex_unlock(&temp_thread->mutex);

		//添加到忙线程队列
		/*add the thread assinged task to the busy queue. */
		pthread_mutex_lock(&pthread_queue_busy->mutex);

		/*if this is the first one in busy queue */
		if (pthread_queue_busy->head == NULL)
		{
			//忙线程队列为空时,只有这一个节点,前后指针都指向它
			pthread_queue_busy->head = temp_thread;
			pthread_queue_busy->rear = temp_thread;
			//当前线程节点的前后都指向空
			temp_thread->prev = temp_thread->next = NULL;
		}
		else
		{
			/*insert in thre front of the queue */
			//头插
			pthread_queue_busy->head->prev = temp_thread;
			temp_thread->prev = NULL;//当前头置空
			temp_thread->next = pthread_queue_busy->head;//向下为原头结点
			pthread_queue_busy->head = temp_thread;//重置真头
			pthread_queue_busy->number++;
		}
		pthread_mutex_unlock(&pthread_queue_busy->mutex);

		/*signal the child thread to exec the work */
		//通知这个被阻塞的线程去执行
		pthread_cond_signal(&temp_thread->cond);
	}
}

/*
 *code to process the new client in every chilld pthead.
 *ptr: the fd come from listen thread that can communicate to the client.
 *return:nothing. void * only used to avoid waring.
 */
//处理已经连接的客户端,在任务管理线程中绑定
//发送文件或者文件属性
void *prcoess_client(void *ptr)
{
	int net_fd;
	net_fd = atoi((char *)ptr);

	struct info client_info;

	memset(&client_info, '\0', sizeof(client_info));

	/*get the command mesg from client */
	if (-1 == recv(net_fd, &client_info, sizeof(client_info), 0))
	{
		printf("recv msg error\n");
		close(net_fd);
		goto clean;
	}

	/* if the client requre the attribute of one file. */
	//如果是请求当前文件属性
	if (client_info.flag == 1)
	{
		struct stat mystat;
		if (-1 == stat(client_info.buf, &mystat))//获得文件状态
		{
			printf("stat %s error\n", client_info.buf);
			close(net_fd);
			goto clean;
		}

		char msgbuf[1024];

		memset(msgbuf, '\0', 1024);
		sprintf(msgbuf, "%d",
				htonl(mystat.st_size)); /*only send the length of the file. */

		/*send info of the file */
		if (-1 == send(net_fd, msgbuf, strlen(msgbuf) + 1, 0))
		{
			printf("send msg error\n");
			close(net_fd);
			goto clean;
		}

		close(net_fd);
		return NULL;
	}
	else//如果是请求文件
	{
		int local_begin =
			ntohl(client_info.local_begin);		/* the content begining location. */
		int length = ntohl(client_info.length); /* the length of the content. */

		int file_fd;
		/* open the src file and get the real content. */
		if (-1 == (file_fd = open(client_info.buf, O_RDONLY)))
		{
			printf("open file %s error\n", client_info.buf);
			close(net_fd);
			goto clean;
		}

		/*seek the read local to the real location. */
		//将文件起始位置给local_begin
		lseek(file_fd, local_begin, SEEK_SET);

		int need_send = length;
		int ret;

		do //去读取从local_begin开始,大小为length长度的文件内容
		{
			char buf[1024];

			memset(buf, '\0', 1024);

			/*read 1024bytes from file everytome, if the last one,read the real
			 * length. */
			//每次最多读取1024,循环读取
			if (need_send < 1024)
				ret = read(file_fd, buf, need_send);
			else
				ret = read(file_fd, buf, 1024);

			if (ret == -1)
			{
				printf("read file %s error\n", client_info.buf);
				close(net_fd);
				close(file_fd);
				return NULL;
			}

			/*send the data to client. */
			//发送数据包
			if (-1 == send(net_fd, buf, ret, 0))
			{
				printf("send file %s error\n", client_info.buf);
				close(net_fd);
				close(file_fd);
				return NULL;
			}
		} while (ret > 0);

		close(net_fd);
		close(file_fd);
	}
	return NULL;

clean:
	sys_clean();
}

/*
 *task_manager: get new task and add to the tail of the task_link.
 *ptr: no used. just avoid warning.
 *return:nothing.
 */
//如果有新的连接到达,创建新的任务节点,将sockfd作为参数传递给任务节点
//网络编程三步走socket->sockaddr_in->bind->listen
void *task_manager(void *ptr)
{
	int listen_fd;

	if (-1 == (listen_fd = socket(AF_INET, SOCK_STREAM, 0)))
	{
		perror("socket");
		goto clean;
	}
	//用于socket ioctl的接口请求结构
	struct ifreq ifr;
	//设置设备名称
	strcpy(ifr.ifr_name, "lo");
	//获得ip,给ifr
	if (ioctl(listen_fd, SIOCGIFADDR, &ifr) < 0)
	{
		perror("ioctl");
		goto clean;
	}

	struct sockaddr_in myaddr;
	//设置sockaddr_in对象三语句
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(PORT);
	myaddr.sin_addr.s_addr =
		((struct sockaddr_in *)&(ifr.ifr_addr))->sin_addr.s_addr;
	//绑定
	if (-1 == bind(listen_fd, (struct sockaddr *)&myaddr, sizeof(myaddr)))
	{
		perror("bind..");
		goto clean;
	}
	//最多监听5个
	if (-1 == listen(listen_fd, 5))
	{
		perror("listen");
		goto clean;
	}

	/*i is the id of the task */
	int i;
	for (i = 1;; i++)
	{
		int newfd;
		struct sockaddr_in client;
		socklen_t len = sizeof(client);
		printf("accepted\n");
		//没有新连接将阻塞
		if (-1 == (newfd = accept(listen_fd, (struct sockaddr *)&client, &len)))
		{
			perror("accept");
			goto clean;
		}
		printf("accepted\n");

		TASK_NODE *temp = NULL;
		TASK_NODE *newtask = (TASK_NODE *)malloc(sizeof(TASK_NODE));
		if (newtask == NULL)
		{
			printf("malloc error");
			goto clean;
		}
		/*
		 *initial the attribute of the task.
		 *because this task havn't add to system,so,no need lock the mutex.
		 */
		//没有加入到任务队列,无需锁定互斥锁,这里参数空间设置为宏最好
		newtask->arg = (void *)malloc(128);

		memset(newtask->arg, '\0', 128);
		//将accept到的fd传递给任务(线程)参数
		sprintf(newtask->arg, "%d", newfd);
		//线程函数指针指向执行函数
		newtask->fun = prcoess_client;
		newtask->tid = 0;
		newtask->work_id = i;
		newtask->next = NULL;
		//初始化互斥锁
		pthread_mutex_init(&newtask->mutex, NULL);
		//加入到队列前锁定队列
		/*add new task to task_link */
		pthread_mutex_lock(&task_queue_head->mutex);

		/*find the tail of the task link and add the new one to tail */
		temp = task_queue_head->head;
		//头为空就直接放在头上
		if (temp == NULL)
		{
			task_queue_head->head = newtask;
		}
		else
		{
			//移动到空尾巴
			while (temp->next != NULL)
				temp = temp->next;
			//追加
			temp->next = newtask;
		}
		task_queue_head->number++;
		//解锁队列
		pthread_mutex_unlock(&task_queue_head->mutex);

		//当任务队列空时,管理线程在任务队列阻塞,唤醒任务队列的管理线程
		/*signal the manager thread , no task coming */
		pthread_cond_signal(&task_queue_head->cond);
	}

	return NULL;

clean:
	sys_clean();
}

/*
 *monitor: get the system info
 *ptr: not used ,only to avoid warning for pthread_create
 *return: nothing.
 */
void *monitor(void *ptr)
{
	/*in order to prevent warning. */
	ptr = ptr;

	THREAD_NODE *temp_thread = NULL;

	while (1)
	{
		pthread_mutex_lock(&pthread_queue_busy->mutex);

		/*output the busy thread works one by one */
		//一个一个去查看当前工作
		temp_thread = pthread_queue_busy->head;

		printf("\n*******************************--\n");
		while (temp_thread)
		{
			printf("thread %ld is  execute work_number %d\n", temp_thread->tid,
				   temp_thread->work->work_id);
			temp_thread = temp_thread->next;
		}
		printf("*******************************++\n\n");

		pthread_mutex_unlock(&pthread_queue_busy->mutex);

		sleep(10);
	}

	return NULL;
}

/*
 *sys_clean: clean the system .
 *this function code need to do to make it more stronger.
 */

void sys_clean(void)
{
	printf("the system exit abnormally\n");
	exit(EXIT_FAILURE);
}
