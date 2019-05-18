#pragma once
#include"event_handler.h"
#include"event.h"


class ListenHandler : public EventHandler {
public:
	ListenHandler(Handle fd);
	virtual ~ListenHandler();
	virtual Handle get_handle() const{
		return _listen_fd;
	}
	virtual void handle_read();
	virtual void handle_write();
	virtual void handle_error();
private:
	Handle _listen_fd;
};


