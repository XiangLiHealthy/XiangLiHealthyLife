#include"event_handler.h"
#include"event.h"


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

	static int SendData(const Handle fd, const byte* ptrData, int nLen );
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
