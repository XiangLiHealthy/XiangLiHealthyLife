#include"business_worker.h"
#include"/usr/include/json/json.h"
#include"../net/lib/task_queue.h"
#include<string.h>
#include "../log/log.h"
#include "../business/msg_seq_manager.h"

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

		if (MsgSeqManager::GetInstance().HandleSeq(pRawData) < 0)
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

int BusinessWorker::SendMsg(const void* pRawData, void* ptrjData, const char* strProtoName) 
{
	RawData* pRaw = (RawData*)pRawData;
	const Json::Value* jRet  = (Json::Value*)ptrjData;

	Json::Value root;
	root["proto"] = Json::Value(strProtoName);
	root["data"] = *jRet;

	/*取得json格式的字节流*/
	const char* pStream = root.toStyledString().c_str();

	/*获得字节流的长度*/
	int nLen = strlen(pStream);

	/*发送长度*/
	int nRet = pRaw->SendMsg ( (byte*)&nLen, sizeof(nLen) );
	if(nRet < 0 ) 
	{
		LOG_ERROR("线程ID:%d发送长度失败!", m_pid);
		return -1;
	}

	/*发送数据*/
	if ( pRaw->SendMsg((byte*)pStream, nLen) < 0) 
	{
		LOG_ERROR("线程ID:%d发送字节流失败!\n", m_pid);
		return -1;
	}

	return 0;

}
