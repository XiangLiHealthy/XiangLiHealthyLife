#pragma once
#include<map>
#include"event.h"
#include"event_demultiplexer.h"

class EventHandler;

class ReactorImpl{
public:
	ReactorImpl();
	~ReactorImpl();
	int regist(EventHandler* handle, Event evt);
	void remove(EventHandler* handler);
	void dispatch(int timeout = 0 );

private:
	EventDemultiplexer* _demultiplexer;
	std::map<Handle, EventHandler*> _handlers;
};
