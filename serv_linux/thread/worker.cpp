#include"worker.h"

Worker::Worker() {
	m_pIsShutDown = NULL;
}

Worker::~Worker() {
	

}

void Worker::SetThreadID(pthread_t pid) {
	m_pid = pid;
}

pthread_t Worker::GetThreadID() {
	return m_pid;
}

void Worker::SetShutDown(const volatile bool* bShutDown){
	m_pIsShutDown = bShutDown;
}
