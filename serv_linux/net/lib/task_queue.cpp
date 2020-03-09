#include"task_queue.h"
#include<stdio.h>
#include<pthread.h>
#include<iostream>
#include "../../log/log.h"

/*****************************************************
 *这是一个全局对象,网络接口将完整的消息放在这里,业务
 模块从这个队列获取消息,然后开始解析协议
 * ************************************************/
 TaskQueue g_taskQueue;

 /*******************************************************
  *初始化互斥量
  *
  *
  *
  * **************************************************/
TaskQueue::TaskQueue() {
	m_cond = PTHREAD_COND_INITIALIZER;
	m_lock = PTHREAD_MUTEX_INITIALIZER; 
	m_bExit = false;
}

/************************************************************
 *释放互斥量的资源
 *
 *
 *************************************************************/
TaskQueue::~TaskQueue() {

	cout << "清除任务队列:" << m_taskQueue.size() << "个" << endl;

	pthread_mutex_lock(&m_lock); 

	while(m_taskQueue.size() > 0) {
		RawData* pData = m_taskQueue.front();
		if ( NULL != pData) {
			delete pData;
		}
	}

	pthread_mutex_unlock(&m_lock);

	//m_taskQueue.clear();
}

/***********************************************************************
 *功能:添加已经接收完整的消息包,不能重复添加
 *参数:
 *RawData* data: 网络接口生成的数据对象,每个对象存储的都是完整的消息
 *
 * 返回值:成功返回0
 *
 * **********************************************************************/
int TaskQueue::add(RawData* pData) 
{
	LOG_DEBUG( "int TaskQueue::add(RawData* %x ", pData);

	if(NULL == pData) {
		return -1;
	}

	//锁住任务队列
	pthread_mutex_lock(&m_lock);

	//将消息添加到任务队列
	m_taskQueue.push(pData);

	//解锁
	pthread_mutex_unlock(&m_lock);

	//通知等待线程
	pthread_cond_signal(&m_cond);

	LOG_DEBUG("int TaskQueue::add() end,count:%d \n", m_taskQueue.size());

	return 0;
}

/***********************************************************************
 *功能:业务线程从消息队列中获取一个消息对象 
 *参数:
 *返回值:指向消息对象的指针,获得者需要释放这个内存 
 * **********************************************************************/
RawData* TaskQueue::get() 
{
	//linux pthread_t 是unsigned long
	LOG_DEBUG("线程ID:%lu,RawData::get() start \n", pthread_self());

	RawData* pData = NULL;
	pthread_mutex_lock(&m_lock);

	//一直等待,直到任务队列有任务为止
	while(m_taskQueue.size() < 1 ) 
	{
		if(m_bExit) 
		{
			pthread_mutex_unlock(&m_lock);
			return NULL;
		}

		pthread_cond_wait(&m_cond, &m_lock);
	}

	//获取任务
	pData = m_taskQueue.front();
	m_taskQueue.pop();
	
	//释放锁资源
	pthread_mutex_unlock(&m_lock);

	LOG_DEBUG("线程ID:%lu, RawData::get() end \n", pthread_self());

	return pData;

}

void TaskQueue::notifyAllExit() {
	
	pthread_mutex_lock(&m_lock);
	m_bExit = true;
	pthread_mutex_unlock(&m_lock);

	pthread_cond_broadcast(&m_cond);
}
