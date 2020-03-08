#include"raw_data.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include "../../log/log.h"

RawData::RawData(int nSize) 
{
	m_byData = (byte*)malloc(nSize + 2);
	m_nSize = nSize;

	m_byData[nSize] = 0;//for unicode string
	m_byData[nSize + 1] = 0;
}

RawData::~RawData() 
{
	clear();
}

/****************************************************************************
 *清理内存
 ***************************************************************************/ 
void RawData::clear() 
{
	if (m_byData)
	{
		delete[] m_byData;
		m_byData = nullptr;
	}

	m_hSock = -1;
}


int RawData::append(const byte* byData, int nLen) 
{
	//判断参数指针和数据缓冲是否有效 
	if (NULL == byData || NULL == m_byData) 
	{
		LOG_ERROR("发现空指针! ");
		return -1;
	}


	//判断数据缓冲的长度是否还够
	if(nLen > m_nSize - m_nPos) 
	{
		LOG_ERROR("添加的数据过长:srcLen = %d, bufLen = %d \n", nLen, m_nSize - m_nPos);
		return -1;
	}


	//将数据拷贝到缓冲区
	memcpy(m_byData + m_nPos, byData, nLen);
	m_nPos += nLen;


	return 0;
}

int RawData::getNeedLen() 
{
	return m_nSize - m_nPos;
}

const byte* RawData:: getData() 
{
	return m_byData ;
}

int RawData::getLength() 
{
	return m_nPos;
}

void RawData::setHandle(Handle hSock) 
{
	m_hSock = hSock;
}

Handle RawData::getHandle( ) 
{
	return m_hSock;
}


