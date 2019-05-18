#pragma once
#include<map>
#include"event_handler.h"
#include"event.h"

//class EventHandler;


class EventDemultiplexer{
public:
	EventDemultiplexer(){

	}
	virtual ~EventDemultiplexer(){

	}

	virtual int wait_event(std::map<Handle, EventHandler*>& handlers, int timeout) = 0;
	virtual int  regist(Handle handle, Event evt) = 0;
	virtual int remove(Handle handle) = 0;
};
