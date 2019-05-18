#include"./thread/thread_manager.h"
#include<stdio.h>

int main (int argc, char* argv[]) {
	
	printf("启动服务器......\n");

	ThreadManager threadManager;
	if ( threadManager.start() < 0) {
		printf("启动失败!");
	}

	printf("...........停止服务!");

	return 0;

}
