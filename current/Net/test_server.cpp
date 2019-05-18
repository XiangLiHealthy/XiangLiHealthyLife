#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <iostream> 
#include <errno.h> 
#include "./lib/reactor.h" 
#include "./lib/event_handler.h" 
#include "./lib/listen_handler.h" 
#include "./lib/event.h"

int g_nPort = 6666;

int main() { 

	int listenfd = -1; 
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
		std::cerr << "socket error " << errno << std::endl; exit(-1); 
	} 

	struct sockaddr_in seraddr; 
	seraddr.sin_family = AF_INET; 
	seraddr.sin_port = htons(g_nPort); 
	seraddr.sin_addr.s_addr = htonl(INADDR_ANY); 

	std::cout<<"执行监听,端口:"<< g_nPort << std::endl;
	if (bind(listenfd, (struct sockaddr*)&seraddr, sizeof(seraddr)) < 0) { 
	std::cerr << "bind error " << errno << std::endl; exit(-2); 
	} 

	if (listen(listenfd, 5) < 0) { 
		std::cerr << "listen error " << errno << std::endl; exit(-3); 
	} 

	Reactor& actor = Reactor::get_instance(); 
	EventHandler* handler = new ListenHandler(listenfd); 

	std::cout<<"执行注册:handler=" << handler <<"envent="<< ReadEvent << std::endl;
	actor.regist(handler, ReadEvent); 

	while(true) { 
	actor.dispatch(100); 
	//std::cout << "one loop" << std::endl; 
	} 

	return 0; 
}
