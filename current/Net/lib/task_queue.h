#pragma once

#include<pthread.h>
#include"raw_data.h"
#include<queue>
#include<pthread.h>

using namespace std;

class TaskQueue {
public:
						TaskQueue();
						~TaskQueue();

	int add(RawData*	data);
	RawData* 			get();
	void				notifyAllExit();
private:
	queue<RawData*> 	m_taskQueue;
	
	pthread_cond_t 		m_cond;
	pthread_mutex_t 	m_lock;
	bool				m_bExit;
};

extern TaskQueue		g_taskQueue;
