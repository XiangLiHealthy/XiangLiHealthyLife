#include "listen_handler.h" 
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdio.h> 
#include <new> 
#include <assert.h> 
#include "event_handler.h" 
#include "reactor.h" 
#include "socket_handler.h"
#include<iostream>
#include<arpa/inet.h>
#include<errno.h>
#include<string.h>

ListenHandler::ListenHandler(Handle fd) : _listen_fd(fd) { 
// do nothing 
} 
ListenHandler::~ListenHandler() { 
close(_listen_fd); 
} 
void ListenHandler::handle_read() { 
	printf("ListenHandler::handle_read()\n");

	struct sockaddr_in client_addr;
	socklen_t len = sizeof(sockaddr);

	int fd = accept(_listen_fd,(sockaddr*) &client_addr, &len); 

	printf("接受连接ip:%s\n", inet_ntoa(client_addr.sin_addr) );

	EventHandler* h = new (std::nothrow)SocketHandler(fd); 
	assert(h != NULL); 
	Reactor& r = Reactor::get_instance(); 
	
	r.regist(h, ReadEvent); 
} 
void ListenHandler::handle_write() { 
// do nothing 
} 
void ListenHandler::handle_error() { 
// do nothing 
	printf("监听套接字出错:%s,进程退出!\n", strerror(errno));
	
	close(_listen_fd);
	exit(1); 
}


