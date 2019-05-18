#include<stdio.h>
#include"sys/socket.h"
#include"arpa/inet.h"
#include<iostream>
#include<errno.h>
#include<string.h>
#include"unistd.h"

int main() {
	int socketfd = -1;
	if ( (socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		std::cerr << "socket error " << errno <<std::endl;
		exit(-1);
	}

	struct sockaddr_in seraddr;
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(6666);
	seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect (socketfd, (struct sockaddr*)&seraddr, sizeof(seraddr)) < 0){
		std::cerr << "connect error" << errno << std::endl;
		exit(-2);
	}


	int n = 0;
	char rbuf[64] = {0};
	memset(rbuf, 0, sizeof(rbuf));
	if ( (n = write(socketfd, rbuf, sizeof(rbuf))) < 0 ){
		std::cerr <<"read error" << errno<< std::endl;
		exit(-3);
	}

	char wbuf[64] = {0};
	strcpy(wbuf, "hello world");
	n = read(socketfd, wbuf, strlen(wbuf));

	std::cout <<"send [" << wbuf << " ] reply [ " << rbuf << "]" << std::endl;

	close(socketfd);
	return 0;
}






















