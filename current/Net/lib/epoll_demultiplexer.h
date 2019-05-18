#include<map>
#include"event_handler.h"
#include"event.h"
#include"event_demultiplexer.h"

class EpollDemultiplexer : public EventDemultiplexer {
public:
	EpollDemultiplexer();
	virtual ~EpollDemultiplexer();
	virtual int wait_event(std::map<Handle, EventHandler*>& handlers, int timeout);
	virtual int regist(Handle handle, Event evt);
	virtual int remove(Handle handle);
private:
	int _max_fd;
	int _epoll_fd;
};
