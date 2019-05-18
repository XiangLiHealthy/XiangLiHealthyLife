#include"worker.h"

/****************************************************************************
 *这个类完成业务流程操作.从消息队列中拿出完整的消息对象,然后解析出功能协议,再
 *从注册的协议列表中找到对应的执行操作;协议对象负责详细业务实现,这个类只负责
 *线程的运作和协议的分配
 * *************************************************************************/

class BusinessWorker : public Worker {
	public:
		BusinessWorker();
		~BusinessWorker();
		
		virtual int start();
		virtual int stop();
	protected:
		int SendMsg(const void* pRawData, void* ptrjRet, const char* strProtoName);
};
