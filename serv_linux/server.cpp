#include"./thread/thread_manager.h"
#include<stdio.h>
#include "log/log.h"

int main (int argc, char* argv[]) 
{
	LOG_INFO("start service damon......\n");

	ThreadManager threadManager;
	if ( threadManager.Start() < 0) {
		LOG_ERROR("start service failed!");
	}

	LOG_INFO("...........stop service!");

	return 0;

}
