#include"raw_data.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>

RawData::RawData(int nSize) {

	m_byData = NULL;//初始内存可能被初始化为0xfdfd fdfd
	clear();

	m_byData = (byte*)malloc(nSize + 1);
	m_nSize = nSize;

}

RawData::~RawData() {
	clear();
}

/****************************************************************************
 *清理内存
 ***************************************************************************/ 
void RawData::clear() {
	if( NULL != m_byData) {
		delete[] m_byData;
		m_byData = 0;
	}

	m_nSize = 0;
	m_nPos = 0;
	m_hSock = NULL;
}


int RawData::append(byte* byData, int nLen) {
	printf("%s: %x, %d \n", __PRETTY_FUNCTION__, byData, nLen);

	//判断参数指针和数据缓冲是否有效 
	if (NULL == byData || NULL == m_byData) {
		printf("发现空指针! \n");
		return -1;
	}


	//判断数据缓冲的长度是否还够
	if(nLen > m_nSize - m_nPos) {
		printf("添加的数据过长:srcLen = %d, bufLen = %d \n", nLen, m_nSize - m_nPos);
		return -1;
	}


	//将数据拷贝到缓冲区
	memcpy(m_byData + m_nPos, byData, nLen);
	m_nPos += nLen;


	return 0;
}

int RawData::getNeedLen() {
	return m_nSize - m_nPos;
}

const byte* RawData:: getData() {
	return m_byData;
}

int RawData::getLength() {
	return m_nSize;
}

void RawData::setHandle(Handle hSock) {
	m_hSock = hSock;
}

Handle RawData::getHandle( ) {
	return m_hSock;
}


int RawData:: SendMsg(const byte* pbyData, int nLen) {
	int nRet = write(m_hSock, (char*)pbyData, nLen);
	if ( nRet < nLen) {
		printf("数据发送失败:%s \n", strerror(errno));
		return -1;
	}

	return 0;
}
