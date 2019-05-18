#pragma once 
#include<pthread.h>

class Worker {
	public:
		Worker();
		~Worker();
		virtual int 			start()	 = 0;
		virtual int 			stop()	 = 0;
		void 					SetThreadID(pthread_t pid);
		pthread_t 				GetThreadID();
		void 					SetShutDown(const volatile bool* bShutdown);
	protected:
		pthread_t 				m_pid;
		const volatile bool* 	m_pIsShutDown;
};
