#pragma once 
#include"net_data.h"

/******************************************************************************
 *socketHandler负责解决半包问题,然后将完整的消息包放在这个类里面;socketHandler负责 
 *申请这个对象的内存资源,业务处理类负责处理完业务之后负责释放内存;
 *
 *如果数据未接受完整,数据将不会放到任务队列,只有接受到完整的数据之后才会放到任务队列
 *,然后socketHandler 放弃对这个对象的持有;
 *
 * ***************************************************************************/
class RawData {
public:
	RawData(int nSize);
	~RawData();
	
	//int alloc(int nSize);
	int 		append(const byte* data, int nLen);
	int 		getNeedLen();
	const byte* getData();
	int 		getLength();

	void 		setHandle(Handle hSock);
	Handle 		getHandle();
private:
	void 		clear();
private:
	Handle 		m_hSock;

	byte*	m_byData;
	int m_nSize;
	int m_nPos;
};
