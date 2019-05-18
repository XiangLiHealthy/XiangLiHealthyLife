#pragma once 

#include"worker.h"
#include<pthread.h>

class NetWorker : public Worker {
	public:
		NetWorker();
		~NetWorker();

		virtual int start();
		virtual int stop();
	protected:
		int 		m_listenfd;
};
