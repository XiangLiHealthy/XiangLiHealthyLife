#include"business_worker.h"
#include"/usr/include/json/json.h"
#include"../net/lib/task_queue.h"
#include<string.h>
#include "../log/log.h"
#include "../sequence/msg_seq_manager.h"

BusinessWorker::BusinessWorker() 
{

}

BusinessWorker::~BusinessWorker() {

}

int BusinessWorker::start() 
{
	LOG_ERROR("线程ID:%d %s \n", m_pid, __PRETTY_FUNCTION__);

	while(false == *m_pIsShutDown) 
	{
		/*从消息队列取得一个任务*/
		RawData* pRawData = g_taskQueue.get();
		if ( NULL == pRawData) 
		{
			LOG_ERROR("线程ID:%d, 获取消息NULL! \n", m_pid);
			continue;
		}

		if (msg_seq_manager::MsgSeqManager::GetInstance().HandleSeq(pRawData) < 0)
		{
			LOG_ERROR("seq handle error");
		}
	}

	LOG_ERROR("线程ID:%d,退出业务工作.....\n", m_pid);

	return 0;
}


int BusinessWorker::stop() 
{
	g_taskQueue.notifyAllExit();
	
	return 0;
}
