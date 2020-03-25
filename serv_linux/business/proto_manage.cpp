#include"proto_manager.h"
#include<stdio.h>
#include "../log/log.h"
#include "clinics.h"
#include "test.h"

ProtoManager g_protoManagerObj;


/************************************************************************
 *下构造函数自动注册所有业务协议.不用再另外执行专门的注册动作,这样可以
 *简化实现;
 *
 *每个新定义的协议都要在这里添加注册代码
 *协议中不能保存业务缓存,因为每个协议可能同时被多个线程执行,对应着不同的
 *连接
 * *******************************************************************/
ProtoManager::ProtoManager() 
{
	//每个协议都是一个单列.并实例化一个静态对象,这样协议名可以每个对象单独维护
	regist(new Clinics());
	regist(new Test());
}

ProtoManager::~ProtoManager() 
{
	for (auto itr : m_mapProto)
	{
		if (itr.second)
		{
			delete itr.second;
		}
	}
	m_mapProto.clear();

}

/************************************************************************
 功能:通过协议名称寻找协议对象
 参数介绍:从协议中取得的协议名称
 返回:协议对象的引用
 详细介绍:因为对于列表只是多线程读,所以不用加锁
***********************************************************************/
const Proto* ProtoManager:: GetProto(const string& strName)
 {
	LOG_DEBUG("get proto");

	//判断参数是否合法
	LOG_DEBUG("name = %s ", strName.c_str());
	
	//通过名字查找对象
	std::map<string , const Proto*>::iterator iter = m_mapProto.find(strName);
	if (iter != m_mapProto.end()) 
	{
		return iter->second;
	}

	//print all protocol name
	PrintAllProtocols();

	return NULL;
}

void ProtoManager::regist(Proto* ptrProto)
{
	if (nullptr == ptrProto)
	{
		LOG_ERROR("invalid para null");
		return ;
	}

	if (m_mapProto.find(ptrProto->getName()) != m_mapProto.end())
	{
		LOG_INFO("proto:%s has be exist", ptrProto->getName().c_str());
		delete ptrProto;
		return;
	}
	
	m_mapProto[ptrProto->getName()] = ptrProto;
}


void ProtoManager::PrintAllProtocols()
{
	int index = 0;
	for (auto itr : m_mapProto)
	{
		LOG_INFO("(%d/%d) protoclol:%s",  m_mapProto.size(), index ++, itr.first.c_str());
	}
}