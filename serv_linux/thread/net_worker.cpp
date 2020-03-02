#include"net_worker.h"
#include<sys/socket.h>
#include<arpa/inet.h>
#include<iostream>
#include<errno.h>
#include<string.h>
#include"../Net/lib/reactor.h"
#include"../Net/lib/event_handler.h"
#include"../Net/lib/listen_handler.h"
#include"../Net/lib/event.h"
#include<stdio.h>
#include<unistd.h>
#include"../Net/lib/task_queue.h"
#include "../log/log.h"

NetWorker::NetWorker() {
	m_listenfd = 0;
}

NetWorker::~NetWorker() {
	if(m_listenfd > 0) {
		close(m_listenfd);
		m_listenfd = -1;
	}
}

/***************************************************************
 *功能介绍:这里启动网络相关的所有接口,绑定ip和端口,使用epoll轮 
 *询消息,接收到消息之后将消息放到任务队列;直到服务器停止才返回 
 *
 *
 * ***********************************************************/
int NetWorker::start() {
	/*创建tcp流套接字描述符*/
	 m_listenfd = -1;
	if ( (m_listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		LOG_ERROR("socket error:%s \n", strerror(errno));
		return -1;
	}
	
	//设置端口可以立即重用 added by 2019-2-21
	int reuse = 1;
	if( setsockopt( m_listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof( int)) < 0) {
		LOG_ERROR( " socket error:%s \n", strerror( errno));
		return -1;
	}	

	/*绑定ip和端口,应该把做到配置文件里面*/
	struct sockaddr_in seraddr;
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(6666);
	seraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(m_listenfd, (struct sockaddr*)&seraddr, sizeof(seraddr)) != 0) {
		LOG_ERROR("bind error: %s \n",strerror(errno));
		close(m_listenfd);
		return -1;
	}

	/*开始监听连接*/
	if ( listen(m_listenfd, 20) < 0) {
		LOG_ERROR("listen error:%s \n", strerror(errno));
		close(m_listenfd);
		return -1;
	}


	/*使用reactor处理连接和接收消息*/
	Reactor& reactor = Reactor::get_instance();
	EventHandler* handler = new ListenHandler(m_listenfd);
	reactor.regist(handler, ReadEvent);

	LOG_ERROR("开始轮询网络连接!\n");
	while(*m_pIsShutDown == false) {
		LOG_ERROR("我还在干活......\n");

		reactor.dispatch(1000);
	}

	LOG_ERROR("网络线程退出.........\n");

	return 0;
}

/**********************************************************
 *功能介绍:直接关闭监套接字,这样就会给epoll发送关闭消息,
 *中断阻塞;listenHandler关闭所有的套接字.然后关闭epoll;
 *但是等待在消息队列上的线程可能还是没有被叫醒,这个工作还是
 *交给业务工作者去处理;
 *
 * *******************************************************/ 
int NetWorker:: stop() {
	if(m_listenfd > 0) {
		close(m_listenfd);
		m_listenfd = 0;
	}


	LOG_ERROR("关闭监听套接字!\n");
	return 0;
}
