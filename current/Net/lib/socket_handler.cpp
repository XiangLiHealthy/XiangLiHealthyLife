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

SocketHandler::SocketHandler(Handle fd) : _socket_fd(fd){
	_buf = new (std::nothrow)char[MAX_SIZE];
	assert (_buf != NULL);
	memset(_buf, 0, MAX_SIZE);

	m_data = NULL;
	m_bFirst = true;
	m_nRecLen = 0;
}

SocketHandler::~SocketHandler() {
	close(_socket_fd);
	delete[] _buf;
}

/***************************************************************************
 *功能介绍:接收完整的字节流,先接收长度信息,然后在读取长度指定的数据;
 *参数:无
 *返回值:无
 *
 *
 *
 * ***********************************************************************/
void SocketHandler::handle_read() {

	printf("%s \n", __PRETTY_FUNCTION__);

	//如果是首次接收数据就先读取4个字节,取得数据包的长度信息
	int nLen = 0;
	if(m_bFirst || NULL == m_data) {
		if( (nLen = read(_socket_fd, _buf + m_nRecLen, 4 - m_nRecLen)) < 0) {
			printf("读消息长度出错:%s \n", strerror(errno));
			handle_error();
			return;
		}
		if( 0 == nLen) {
			printf( " socket=%d，连接断开；\n", _socket_fd);
			handle_error( );
			return;
		}

		printf("socket=%d，接收到字节数：%d \n", _socket_fd, nLen);

		m_nRecLen += nLen;
		if( 4 == m_nRecLen ){
			int nMsgLen = * ( (int*)_buf);
			m_data = new RawData(nMsgLen);
			if( NULL == m_data) {
				printf("内存不足,结束进程 \n");
				handle_error();
				return;
			}

			m_bFirst = false;
		}
		//直接再读一次可能会阻塞
		return ;
	}


	//如果不够4字节,直到取得到为止
	//创建一个RawData对象,并初始化缓冲区
	//读取指定数目的长度:
	int nNeedLen = m_data->getNeedLen();
	nLen = (nNeedLen > MAX_SIZE) ? MAX_SIZE : nNeedLen;
	if ( (nLen = read(_socket_fd, _buf, nNeedLen) ) < 0) {
		printf("接收消息内容出错:%s \n", strerror(errno));
		handle_error();
		return;
	}
	if( 0 == nLen) {
		printf( " socket=%d，连接断开；\n", _socket_fd);
		handle_error( );
		return;
	}
	
	printf("socket=%d,len=%d:%s \n", _socket_fd, nLen, _buf);

	//将读取的数据添加到消息对象
	m_data->append( (byte*)_buf, nLen);

	//数据接收够了就将这个对象放到任务队列,下一次重新生成一个对象
	if (0 == m_data->getNeedLen()) {
		g_taskQueue.add(m_data);
		m_data = NULL;
		m_bFirst = true;
		m_nRecLen = 0;
	}

	//LT模式 如果还有消息剩余会继续:得到通知,直到把字节流接收完
}

void SocketHandler::handle_write() {
	//do nothing
}

void SocketHandler::handle_error() {
	//Reactor& r = Reactor::get_instance();
	//r.remove(this);
	close(_socket_fd);
	//_sock_fd = NULL;
}














































