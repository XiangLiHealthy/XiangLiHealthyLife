#include"proto_manager.h"
#include<stdio.h>
#include "../log/log.h"

ProtoManager g_protoManagerObj;


/************************************************************************
 *下构造函数自动注册所有业务协议.不用再另外执行专门的注册动作,这样可以
 *简化实现;
 *
 *每个新定义的协议都要在这里添加注册代码
 *协议中不能保存业务缓存,因为每个协议可能同时被多个线程执行,对应着不同的
 *连接
 * *******************************************************************/
ProtoManager::ProtoManager() {
	//每个协议都是一个单列.并实例化一个静态对象,这样协议名可以每个对象单独维护
	//Proto*  ptr = clinics.getInstance();
	//m_mapProto.add(ptr->getName(), ptr);
	
	
}

ProtoManager::~ProtoManager() {

}

/************************************************************************
 功能:通过协议名称寻找协议对象
 参数介绍:从协议中取得的协议名称
 返回:协议对象的引用
 详细介绍:因为对于列表只是多线程读,所以不用加锁
***********************************************************************/
const Proto* ProtoManager:: GetProto(const char* szName) {
	LOG_DEBUG("get proto");

	//判断参数是否合法
	if(NULL == szName) {
		LOG_ERROR("非法参数NULL ");
		return NULL;
	}

	LOG_DEBUG("name = %s ", szName);
	
	//通过名字查找对象
	std::map<const char* , const Proto*>::iterator iter = m_mapProto.find(szName);
	if (iter != m_mapProto.end()) 
	{
		return iter->second;
	}

	return NULL;
}

