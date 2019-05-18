#include"business_worker.h"
#include"/usr/include/json/json.h"
#include"../Net/lib/task_queue.h"
#include"../business/proto_manager.h"
#include"../business/proto.h"
#include<string.h>

BusinessWorker::BusinessWorker() {

}

BusinessWorker::~BusinessWorker() {

}

int BusinessWorker::start() {
	printf("线程ID:%d %s \n", m_pid, __PRETTY_FUNCTION__);

	DataBase db;

	//地址，数据库名称，用户，密码
	if( !db.Connect("127.0.0.1","HealthyLife", "xiangbaosong", "Xl2016xl") ) {
		printf("线程ID:%d,数据库连接失败:%s \n", m_pid, db.GetLastError());
		return -1;
	}

	while(false == *m_pIsShutDown) {
		/*从消息队列取得一个任务*/
		RawData* pRawData = g_taskQueue.get();
		if ( NULL == pRawData) {
			printf("线程ID:%d, 获取消息NULL! \n", m_pid);
			continue;
		}

		/*转化成json对象*/
	   const byte* pbyData = pRawData->getData();
	   if ( NULL == pbyData) {
		   printf("线程ID:%d,消息内数据为NULL!\n", m_pid);
		   delete pRawData;
		   continue;
	   }

	   Json::Reader 	reader;
	   Json::Value 		jData;
	   if ( reader.parse( (char*)pbyData, jData) < 0) {
			printf("线程ID=%d, 消息转化json失败!", m_pid);
			delete pRawData;
			continue;
	   }

		/*取得协议名称*/
		if (jData["proto"].isNull()) {
			printf("线程ID:%d, 非法协议,字段中没有proto:%s \n", m_pid, pbyData);
			delete pRawData;
			continue;
		}
		const string strName =  jData["proto"].asString();
		
		/*取得协议对象*/
		Proto* ptrProto = (Proto*)g_protoManagerObj.GetProto(strName.c_str());
		if (NULL == ptrProto) {
			printf("线程ID:%d, 未找到协议:%s \n", m_pid, strName.c_str());
			delete pRawData;
			continue;
		}

		/*协议对象执行业务逻辑,返回回复给客户端的json*/
		printf("线程ID%d,执行协议:%s!\n", m_pid, strName.c_str());

		/*将json对象转化成字节流,发送给客户端*/
		Json::Value jRet = ptrProto->dispatch(jData, &db);
		if ( 0 != SendMsg(pRawData, &jRet, strName.c_str())) {
			printf("线程ID:%d,发送消息失败!\n", m_pid);
		}

		delete pRawData;
	}

	printf("线程ID:%d,退出业务工作.....\n", m_pid);

	return 0;
}


int BusinessWorker::stop() {
	g_taskQueue.notifyAllExit();
	
	return 0;
}

int BusinessWorker::SendMsg(const void* pRawData, void* ptrjData, const char* strProtoName) {
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
	if(nRet < 0 ) {
		printf("线程ID:%d发送长度失败!", m_pid);
		return -1;
	}

	/*发送数据*/
	if ( pRaw->SendMsg((byte*)pStream, nLen) < 0) {
		printf("线程ID:%d发送字节流失败!\n", m_pid);
		return -1;
	}

	return 0;

}
