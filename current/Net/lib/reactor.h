#pragma once
#include"event_handler.h"
#include"event.h"
#include"event_handler.h"
#include"reactor_impl.h"

class ReactorImpl;//隐藏实现细节

class Reactor
{
public:
	Reactor();
	~Reactor();

	static Reactor& get_instance();
	int regist(EventHandler* handler, Event evt);
	void remove(EventHandler* handler);
	void dispatch(int timeout = 0);

private:
	Reactor(const Reactor&);
	Reactor& operator=(const Reactor&);

private:
	ReactorImpl* _reactor_impl;
	static Reactor reactor;
};
