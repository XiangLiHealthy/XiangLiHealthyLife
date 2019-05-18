#include"event.h"
#include"reactor.h"
#include<assert.h>
#include<new>

Reactor Reactor::reactor;



Reactor::Reactor(){
	_reactor_impl = new (std::nothrow)ReactorImpl();
	assert(NULL != _reactor_impl);
}

Reactor::~Reactor(){
	if(NULL != _reactor_impl){
		delete _reactor_impl;
		_reactor_impl = NULL;
	}
}

Reactor& Reactor::get_instance(){
	return reactor;
}

int Reactor::regist(EventHandler* handler, Event evt){
	return _reactor_impl->regist(handler, evt);
}

void Reactor::remove(EventHandler* handler){
	return _reactor_impl->remove(handler);
}

void Reactor::dispatch(int timeout){
	return _reactor_impl->dispatch(timeout);
}
