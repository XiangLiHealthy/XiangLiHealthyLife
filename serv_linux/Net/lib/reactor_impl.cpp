#include"reactor_impl.h"
#include<new>
#include<assert.h>
#include"epoll_demultiplexer.h"
#include"event_handler.h"
#include<iostream>
#include"net_data.h"
#include "../../log/log.h"

ReactorImpl::ReactorImpl(){
	_demultiplexer = new (std::nothrow)EpollDemultiplexer;
	assert(_demultiplexer != NULL);
}

ReactorImpl::~ReactorImpl(){
	std::map<Handle, EventHandler*>::iterator iter = _handlers.begin();
	for(; iter != _handlers.end(); ++iter){
		delete iter->second;
	}

	if (NULL != _demultiplexer){
		delete _demultiplexer;
	}
}

int ReactorImpl::regist(EventHandler* handler, Event evt){
	LOG_DEBUG("reactor::regist(EventHandler=%ox, Event=%d)\n",handler, evt);

	Handle handle = handler->get_handle();
	//const Handle handle = handler->getHandle();
	if (_handlers.find(handle) == _handlers.end()){
		_handlers.insert(std::make_pair(handle, handler));
	}

	return _demultiplexer->regist(handle, evt);
}

void ReactorImpl::remove(EventHandler* handler){
	LOG_DEBUG("ReactorImpl::remove(EventHandler* %x)", handler);

	Handle handle = handler->get_handle();
	//Handle handle = handler>getHandle();

	_demultiplexer->remove(handle);//not check?

	//added by xbs 2018-12-24
	//对象资源都释放了,应该主动关闭文件描述符,否则会造成资源泄露,客户端一直收不到反应
	//close(handle);

	//std::cout << "reactor_impl 引起崩溃" << std::endl;
	//make sure you can find handle?i think shuld add a juge
	std::map<Handle, EventHandler*>::iterator iter = _handlers.find(handle);
	if(iter != _handlers.end())
	{
		delete iter->second;
		_handlers.erase(iter);
	}
}

void ReactorImpl::dispatch(int timeout){
	_demultiplexer->wait_event(_handlers, timeout);
}
