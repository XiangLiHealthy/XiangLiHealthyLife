#pragma once 

#include<vector>

class Worker;

class ThreadManager{
	public:
		ThreadManager();
		~ThreadManager();

		int 					Start();
		int 					Stop();
		static void				WaitSignal();
		static void 			BlockSignal();

	private:
		//void clear();
		std::vector<Worker*> 	m_threadWorker;
		static void* 			CallBack(void* pPara);
		volatile bool  			m_bIsShutdown;//原子操作比较保险,免得其它线程会把缓存在寄存器中
};
