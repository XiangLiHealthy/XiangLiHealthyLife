#include"thread_manager.h"
#include"worker.h"
#include"net_worker.h"
#include"business_worker.h"
#include<stdio.h>
#include<pthread.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>

#include "../log/log.h"
/**********************************************************
 *需要安装一个信号处理函数来重启或者停止
 *
 *
 * ******************************************************/ 
ThreadManager::ThreadManager() {
	m_bIsShutdown = false;
}

ThreadManager::~ThreadManager() {

}

/*********************************************************
 *功能介绍	：启动一个线程负责监听连接，cpu核心数2倍线程数负
 *			  责业务处理，主线程负责进程信号；
 *参数		：无；
 *返回值	：0 成功，非0表示失败；
 *********************************************************/
int ThreadManager::Start() {
	LOG_ERROR("%s \n", __PRETTY_FUNCTION__);

	//也许这里可以使用抽象工厂设计模式
	//使用一个线程来维护网络连接和字节流读取
	Worker* ptr = new NetWorker;
	if ( NULL == ptr) {
		LOG_ERROR("内存不足!\n");
		return -1;
	}
	m_threadWorker.push_back(ptr);

	//使用cpu核心数量的2倍来处理业务逻辑//这个数量并不能使得
	//cpu的资源被充分利用,因为业务线程还有磁盘io和数据库网络IO .
	//这些IO文件描述符都没有使用epoll
	int nNumProcessor 	= sysconf( _SC_NPROCESSORS_CONF);
	int nEable 			= sysconf( _SC_NPROCESSORS_ONLN);
	LOG_ERROR("系统cpu核心总数:%d, 可用cpu核心数:%d \n", nNumProcessor, nEable);

	for(int nIndex = 0; nIndex < 2*nEable; nIndex++) {
		ptr = new BusinessWorker;
		if ( NULL == ptr) {
			LOG_ERROR("内存不足!\n");
			Stop();
			return -1;
		}
		m_threadWorker.push_back(ptr);
	}


	for(int index = 0; index < m_threadWorker.size(); index++) {
		pthread_t pid = 0;

		ptr = m_threadWorker[index];
		ptr->SetShutDown(&m_bIsShutdown);
		
		int ret  = pthread_create(&pid, NULL, CallBack, ptr);
		if(0 != ret) {
			LOG_ERROR("创建线程失败:%s \n", strerror(ret));
			Stop();
			return -1;
		}

		ptr->SetThreadID(pid);
	}

	//主线程不能退出,否则可能根据实现不同而进程终止;让主线程来出现信号
	//子线程屏蔽所有的信号
	WaitSignal();

	return 0;
}

/*************************************************************************
 *功能介绍:处理信号,交给主线程来做
 *
 *
 * **********************************************************************/
void ThreadManager::WaitSignal() {
	sigset_t sig;
	int err, signo;

	sigemptyset(&sig);
	sigaddset(&sig, SIGHUP);
	sigaddset(&sig, SIGINT);
	sigaddset(&sig, SIGQUIT);

	while(1) {
		err = sigwait(&sig, &signo);

		//发生错误直接退出进程
		if( 0 != err) {
			LOG_ERROR("sigwait fail %s !exit........\n", strerror(err));
			//stop();
			break;
		}

		switch(signo)
		{
			case SIGINT:
				{
					LOG_ERROR("interrupt \n");
				}break;

				//退出程序
			case SIGQUIT:
				{
					LOG_ERROR("quit \n");
				}break;

				//更新配置文件
			case SIGHUP:
				{
					LOG_ERROR("sighup \n");
				}break;
			default:
				{
					LOG_ERROR("default \n");
					exit(1);
				}
		}
	}
}


/**********************************************************************
 *子线程阻塞所有信号,免得对应的信号被当做默认方式处理导致服务器退出

 * *******************************************************************/ 
void ThreadManager::BlockSignal() {

	sigset_t sig;
	sigfillset(&sig);

	int nErr = pthread_sigmask(SIG_BLOCK, &sig, NULL);
	if (0 != nErr) {
		LOG_ERROR("SIG_BLOCK error %s \n", strerror(nErr));
	}
}

void* ThreadManager::CallBack(void* pObj) {
	if(NULL == pObj) {
		return NULL;
	}

	//使用动态转换保证绝对安全.感觉有点多此一举
	Worker* ptrWorker =  (Worker*) pObj;
	if ( NULL == ptrWorker) {
		return NULL;
	}

	//阻塞所有信号,把信号交给主线程去处理
	BlockSignal();

	//在start里面循环
	 ptrWorker->start();

	 return NULL;
}

int ThreadManager::Stop() {
	m_bIsShutdown = true;


	//等待所以线程都正确退出
	//所有线程都等待在消息队列上,需要先把条件设置为满足,然后 
	//叫醒所有线程,不然写线程退出之后其它线程永远无法醒来
	//队列上应该先判断开关是否开着,然后才是判断队列是否为空
	

	return 0;
}
