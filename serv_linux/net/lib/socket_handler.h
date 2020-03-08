#include"event_handler.h"
#include"event.h"
struct send_frame_t;


class RawData;
class SocketHandler : public EventHandler 
{
public:
	SocketHandler(Handle fd);
	virtual ~SocketHandler();
	virtual Handle get_handle() const 
	{
		return _socket_fd;
	}
	virtual void handle_read() override;
	virtual void handle_write() override;
	virtual void handle_error() override;

	static int SendFrame(const Handle fd, send_frame_t& frame_data);
private:
	Handle _socket_fd;
	static const int MAX_SIZE = 8192;
	byte m_buf[MAX_SIZE];
	int m_header_pos;
	int m_tail_pos;

private:
	int ParserBuffer();
	int ParserFrame(const int nHeaderPos, const int nTailPos); 
	int GetHeaderPos();
	int GetTailPos();
	bool CheckData();
};
