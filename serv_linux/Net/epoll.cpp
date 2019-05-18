#include<iostream>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

using namespace std;
#define MAXLINE 5
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 5000
#define INFTIM 1000

int main(int argc,char* arg[])
{
	int listen_fd, connfd_fd, socket_fd,epfd, nfds;
	ssize_t n = 0;
	char line[MAXLINE];
	socklen_t clienlen;
	
	//声明epool_event结构体变量,en用于回传事件,数组用于魂穿要处理的事件
	struct epoll_event ev,events[20];

	//生成处理用于处理accept的epoll专用的文件描述符
	//认为这里应该是生成一个epoll描述符,类似windows的完成端口句柄
	
	cout<<"1.创建epoll"<<endl;

	epfd = epoll_create(5);
	if(epfd < 0)
	{
		printf("epoll文件描述符创建错误:%s",strerror(errno));
		exit(0);
	}

	//接下来才是生成专门接受链接的套接字描述符
	struct sockaddr_in clientaddr;
	struct sockaddr_in serveraddr;

	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_fd < 0)
	{
		cout<<"创建监听套接字失败:"<<strerror(errno)<<endl;
		exit(0);
	}

	//设置与要处理的事件相关的描述符
	ev.data.fd = listen_fd;

	//设置要处理事件的类:
	ev.events = EPOLLIN|EPOLLET;

	//注册epoll事件
	cout<<"注册监听描述符到epoll"<<endl;

	epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev);


	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERV_PORT);

	if(-1 == bind(listen_fd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) )
	{
		printf("bind socket error: %s(erno = %d)\n",strerror(errno), errno);
		exit(0);

	}

	//禁用TIME_WAIT状态
	//int on = 1;
	//if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)))
	//{
	//	cout<<"修改socket选项失败:"<<strerror(errno)<<endl;
	//	exit(0);
	//}

	if(-1 == listen(listen_fd, LISTENQ))
	{
		printf("listen err:%s(errno = %d)",strerror(errno), errno);
		exit(0);
	}

	while(1)
	{
		//等待epoll事件的发生
		nfds = epoll_wait(epfd, events, 20, -1);
		cout<<"epoll得到事件数:"<<nfds<<endl;

		//sleep(1000);

		//处理所有发生的事件
		for(int nIndex = 0; nIndex < nfds; nIndex++)
		{
			cout<<"监听描述符:"<<listen_fd<<"事件描述符:"<<events[nIndex].data.fd<<endl;

			//如果是监听套接字有消息应该是有新用户连接,就创建新的连接
			if(events[nIndex].data.fd == listen_fd)
			{
				connfd_fd = accept(listen_fd, (sockaddr*)&clientaddr, &clienlen);
				if(connfd_fd < 0)
				{
					perror("connfd < 0 ");
					exit(1);

				}

				char *pszIP = inet_ntoa(clientaddr.sin_addr);
				printf("accept a connection from %s", pszIP);

				//设置用于读操作的文件描述符
				ev.data.fd = connfd_fd;

				//设置用于读操作的注册事件
				ev.events = EPOLLIN|EPOLLET;

				//注册ev
				epoll_ctl(epfd, EPOLL_CTL_ADD, connfd_fd, &ev);
			}
			else if(events[nIndex].events & EPOLLIN)
			{
				//收到已经连接的用户发来数据

				if( (socket_fd = events[nIndex].data.fd) < 0 )
				{
					continue;
				}


				memset(&line, 0, sizeof(line));
				if( (n = read(socket_fd, line, MAXLINE)) < 0 )
				{
					if(ECONNRESET == errno)
					{
						close(socket_fd);
						events[nIndex].data.fd = -1;//为什么不从epoll树中删除?

					}
					else
					{
						printf("read lines error: %s(errno + %d)", strerror(errno), errno);

					}
				}
				else if(0 == n)
				{
					close(socket_fd);
					events[nIndex].data.fd = -1;
				}

				printf("%s\n", line);

				//设置用于写操作的描述符
				ev.data.fd = socket_fd;

				//设置用于注册的写操作事件
				ev.events = EPOLLOUT|EPOLLET;

				//执行修改
				//epoll_ctl(epfd, EPOLL_CTL_MOD, socket_fd, &ev);

			}
			else if(EPOLLOUT)
			{
				//如果有数据发送出去
				socket_fd = events[nIndex].data.fd;
				write(socket_fd, line, n);

				//设置读操作描述符和事件
				ev.data.fd = socket_fd;
				ev.events = EPOLLIN|EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_MOD, socket_fd, &ev);
			}
		}
	}

	return 0;

}

