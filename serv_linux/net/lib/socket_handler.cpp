#include"socket_handler.h"
#include<errno.h>
#include"string.h"
#include"stdio.h"
#include"assert.h"
#include"unistd.h"
#include"sys/socket.h"
#include"reactor.h"
#include"task_queue.h"
#include<errno.h>
#include<string.h>
#include "../../log/log.h"
#include "net_data.h"

static const byte FARAME_HEADER[] = {0x11, 0x22};
static const byte  FARAME_TAIL[] = {0x33, 0x44};
static const  short CHECKSUM_SIZE = 2;
static const short FRAME_LEN_SIZE = 2;
static const byte DISCONNECT_FRAME[]= {0x00, 0x01,0x00,  0x01, 0x03};

SocketHandler::SocketHandler(Handle fd) : _socket_fd(fd)
{
	m_header_pos =0;
	m_tail_pos = 0;
}

SocketHandler::~SocketHandler() 
{
	close(_socket_fd);
}

/***************************************************************************
 *功能介绍:接收完整的字节流,先接收长度信息,然后在读取长度指定的数据;
 *参数:无
 *返回值:无
 *
 *
 *
 * ***********************************************************************/
void SocketHandler::handle_read() 
{
	LOG_DEBUG("receive data");

	//read data from tcp buffer
	int nLen = 0;
	if( (nLen = read(_socket_fd, m_buf + m_tail_pos, sizeof(m_buf) - m_tail_pos)) <= 0) 
	{
		LOG_ERROR("read socket,fd:%d, error:%s \n", _socket_fd,  strerror(errno));
		handle_error();
		return;
	}
	m_tail_pos += nLen;

	//parase buffer
	if (ParserBuffer() != 0)
	{
		LOG_ERROR("parser socker buffer error");
	}

	//move leave buffer
	memcpy(m_buf, m_buf + m_header_pos, m_tail_pos - m_header_pos);
	
	m_tail_pos = m_tail_pos - m_header_pos;
	m_header_pos = 0;
}

void SocketHandler::handle_write() 
{
	//do nothing
}

void SocketHandler::handle_error() 
{
	LOG_INFO("socket fd:%d disconnect", _socket_fd);

	RawData* ptrData = new RawData(sizeof(DISCONNECT_FRAME));
	if (nullptr == ptrData)
	{
		LOG_ERROR("no memry");
		return ;
	}

	if (ptrData->append(DISCONNECT_FRAME, sizeof(DISCONNECT_FRAME)) < 0)
	{
		LOG_ERROR("append data error");
		delete ptrData;
		return;
	}

	ptrData->setHandle(_socket_fd);
	g_taskQueue.add(ptrData);

	Reactor& r = Reactor::get_instance();
	r.remove(this);
}

int SocketHandler::ParserBuffer()
{
	do
	{
		int nHeaderPos = GetHeaderPos();
		if (nHeaderPos < 0)
		{
			int nLoseLen = m_tail_pos - m_header_pos -1;
			LOG_ERROR("can not get frame header,lose len:%d", nLoseLen);

			m_buf[0] = m_buf[m_tail_pos -1];
			m_tail_pos = 1;
			m_header_pos = 0;
			
			break;
		}

		m_header_pos = nHeaderPos;

		int nTailPos = GetTailPos();
		if (nTailPos < 0)
		{
			break;
		}

		if (ParserFrame(nHeaderPos, nTailPos) < 0)
		{
			LOG_ERROR("parse frame error");
		}

		m_header_pos = nTailPos;
	} while ( m_tail_pos - m_header_pos > 0);
	
	return 0;
}

int SocketHandler::ParserFrame(const int nHeaderPos, const int nTailPos)
{
	//check data
	if (!CheckData())
	{
		LOG_ERROR("check data error");
		m_header_pos = nTailPos;
		return -1;
	}

	//get data
	int nLen = m_buf[nHeaderPos +  sizeof (FARAME_HEADER) + CHECKSUM_SIZE ] * 256 +
	m_buf[nHeaderPos +  sizeof (FARAME_HEADER) + CHECKSUM_SIZE + 1 ];

	RawData* ptrData = new RawData(nLen);
	if (nullptr == ptrData)
	{
		LOG_ERROR("no memry, need memeory:%d bytes", nLen);
		return -1;
	}

	byte* ptrBuff = &m_buf [nHeaderPos + sizeof (FARAME_HEADER) + CHECKSUM_SIZE + FRAME_LEN_SIZE];
	if (ptrData->append(ptrBuff, nLen) < 0)
	{
		LOG_ERROR("append data error");
		delete ptrData;
		return -1;
	}

	ptrData->setHandle(_socket_fd);
	g_taskQueue.add(ptrData);

	return 0;
}

int SocketHandler::GetHeaderPos()
{
	int nHeaderPos = -1;
	for (int index = 0; index < m_tail_pos - m_header_pos; index ++)
	{
		if (m_buf[m_header_pos + index] == FARAME_HEADER[0] 
		&& m_buf[m_header_pos + index + 1] == FARAME_HEADER[1])
		{
			nHeaderPos = m_header_pos + index;
			break;
		}
	}

	return nHeaderPos;
}

int SocketHandler::GetTailPos()
{
	int nRecvLen = m_tail_pos - m_header_pos;
	if (nRecvLen < FRAME_FORMAT_SIZE)
	{
		return -1;
	}

	int nDataLen = m_buf [m_header_pos + sizeof(FARAME_HEADER) + CHECKSUM_SIZE] * 256 + 
	 m_buf [m_header_pos + sizeof(FARAME_HEADER) + CHECKSUM_SIZE + 1];

	if (nRecvLen < nDataLen + FRAME_FORMAT_SIZE)
	{//has not received tail
		return -1;
	}

	//caculate tail pos
	int nTailPos = m_header_pos + FRAME_FORMAT_SIZE + nDataLen - sizeof(FARAME_TAIL);
	
	//check tail
	if (FARAME_TAIL[0] != m_buf[nTailPos] || FARAME_TAIL[1] != m_buf[nTailPos + 1])
	{
		//invaild frame,throw away these data
		m_header_pos = nTailPos;
		return -1;
	}

	return nTailPos + sizeof(FARAME_TAIL);
}

bool SocketHandler::CheckData()
{
	return true;
}

int SocketHandler::SendFrame(const Handle fd, send_frame_t& frame_data)
{
	 frame_data.ptrFrameHeader[0] = FARAME_HEADER[0];
	 frame_data.ptrFrameHeader[1] = FARAME_HEADER[1];

	* frame_data.ptrCheckSum = 0;//checksum default is 0

	*frame_data.ptrSeqLen = frame_data.nFrameLen - FRAME_FORMAT_SIZE;

	frame_data.ptrFrameTail[0] = FARAME_TAIL[0];
	frame_data.ptrFrameTail[1] = FARAME_TAIL[1];

	int nRet = write(fd, frame_data.byBuff, frame_data.nFrameLen);
	if ( nRet < frame_data.nFrameLen) 
	{
		LOG_ERROR("数据发送失败:%s \n", strerror(errno));
		return -1;
	}

	return 0;
}