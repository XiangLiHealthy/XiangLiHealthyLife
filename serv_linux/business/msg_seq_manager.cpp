#include "msg_seq_manager.h"
#include "../log/log.h"
#include "../net/lib/raw_data.h"
#include "proto_manager.h"
#include "proto.h"
#include <string.h>
#include "../net/lib/socket_handler.h"

enum MSG_TYPE 
{
    TYPE_JSON = 1,
    TYPE_FILE = 2,
    TYPE_DISCONNECT = 3,
};

static const int SEQ_NUMS_SIZE = 2;
static const int SEQ_NUM_SIZE = 2;
static const int SEQ_TYPE_SIZE = 2;

	// 	delete ptrData;
int MsgSeqManager::HandleSeq(RawData* ptrData)
{
    if (nullptr == ptrData)
    {
        LOG_ERROR("null point");
        return -1;
    }

    //get type
    int nRet = -1;
    MSG_TYPE msg_type = (MSG_TYPE)GetDataType(ptrData);
    switch (msg_type)
    {
         case TYPE_JSON:
             {
                 nRet = HandleJson(ptrData);
             }break;

         case TYPE_FILE:
             {
                 nRet = HandleFile(ptrData);
             }break;
          case TYPE_DISCONNECT:
          {
              nRet = HandleDisconnect(ptrData);
          }break;

         default:
             LOG_ERROR("invalid msg type :%d", msg_type);
             break;
    }

    return nRet;
}

 int MsgSeqManager::HandleJson(RawData* ptrData)
 {
     if (nullptr == ptrData)
     {
         LOG_ERROR("null point");
         return -1;
     }

     //weather last sequence
     int nSeqNums = GetSeqNums(ptrData);
     int nSeqNum = GetSeqNum(ptrData);

     auto& seqs = m_msgSeqs[ptrData->getHandle()];
    seqs[nSeqNum] = ptrData;

     if (nSeqNum < nSeqNums)
     {
         return 0;
     }

    do
    {
         //meger seq
        RawData* ptrDataTmp = GetSeq(ptrData->getHandle(), nSeqNums);
        if (nullptr == ptrDataTmp)
        {
            LOG_ERROR("get seq failed");
            break;
        }

         //perform work
        if (PerformWork(ptrDataTmp) < 0)
        {
            LOG_ERROR("perform work error");
        }

        delete ptrDataTmp;
    } while (0);
    
    //clear socket cache
    ClearSeqs(ptrData->getHandle());

    return 0;
 }

int  MsgSeqManager::HandleFile(RawData* ptrData)
{
    if (nullptr == ptrData)
    {
        LOG_ERROR("nullptr");
        return -1;
    }

    LOG_WARNING("not surport file receive");
    return 0;
}

int  MsgSeqManager::PerformWork( RawData* ptrData)
{
    /*转化成json对象*/
	   const byte* pbyData = ptrData->getData();
	   if ( NULL == pbyData) 
	   {
		   LOG_ERROR("nullptr");
		   return -1;
	   }

	   Json::Reader 	reader;
	   Json::Value 		jData;
	   if ( reader.parse( (char*)pbyData + SEQ_FORMAT_SIZE, jData) < 0) 
	   {
			LOG_ERROR("parse json failed!");
			return -1;
	   }

		/*取得协议名称*/
		if (jData["proto"].isNull()) 
		{
			LOG_ERROR("thers is not key proto");
            return -1;
		}
		const string strName =  jData["proto"].asString();
		
		/*取得协议对象*/
		Proto* ptrProto = (Proto*)g_protoManagerObj.GetProto(strName.c_str());
		if (NULL == ptrProto) 
		{
			LOG_ERROR("there is not : %s ",  strName.c_str());
			return -1;
		}

		/*协议对象执行业务逻辑,返回回复给客户端的json*/
		LOG_DEBUG("perform protocol :%s!",  strName.c_str());

		/*将json对象转化成字节流,发送给客户端*/
		if (  ptrProto->dispatch(jData, ptrData->getHandle() ) < 0) 
		{
			LOG_ERROR("perform business failed");
            return -1;
		}

        return 0;
}

RawData* MsgSeqManager::GetSeq(const Handle fd, int nSeqNums)
{
    //check seq nums
    auto& seqs = m_msgSeqs[fd];
    if (seqs.size() != nSeqNums)
    {
        LOG_ERROR("lose seq,need:%d, received:%d", nSeqNums, seqs.size());
       return nullptr;
    }

    //merger data
    //caculat all data len
    int nLen = 0;
    for (auto itr: seqs)
    {
        nLen += itr.second->getLength() - SEQ_FORMAT_SIZE;
    }
    nLen += SEQ_FORMAT_SIZE;

    //create a bigger RawData obj
    RawData* ptrDataTmp = new RawData(nLen);
    if (nullptr == ptrDataTmp)
    {
        LOG_ERROR("no memry");
        return nullptr;
    }

    //fill seq header
    byte HEADER[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    ptrDataTmp->append(HEADER, sizeof(HEADER));

    //move data to the new obj
    for (int index = 0; index < nSeqNums; index ++ )
    {   
        auto itr = seqs.find(index);
        if ( itr == seqs.end())
        {
            LOG_ERROR("there is not seq:%d", index);
            delete ptrDataTmp;
            ptrDataTmp = nullptr;
           break;
        }

        const byte* ptrBuff = itr->second->getData();
        int nLenTmp = itr->second->getLength();

        ptrDataTmp->append(ptrBuff + SEQ_FORMAT_SIZE , nLenTmp + SEQ_FORMAT_SIZE);
    }

    return ptrDataTmp;
}

int MsgSeqManager::HandleDisconnect(RawData* ptrData)
{
    ClearSeqs(ptrData->getHandle());

    m_msgSeqs.erase(ptrData->getHandle());

    return 0;
}

void MsgSeqManager::ClearSeqs(const Handle fd)
{
    auto seqs = m_msgSeqs[fd];
    for(auto itr:seqs)
    {
        if (itr.second)
        {
            delete itr.second;
        }
    }
}

int MsgSeqManager::SendData(Handle fd, int nType, int nTotalLen, int nLen, const byte* ptrBuff)
{
    if (nullptr == ptrBuff || nLen < 0 || nTotalLen < nLen)
    {
        LOG_ERROR("invalid para, ptrBuff:%p, nTotalLen:%d, nLen:%d", ptrBuff, nTotalLen, nLen);
        return -1;
    }

    //send once
    int nSeqNums = nTotalLen / MAX_DATA_SIZE;
    nSeqNums = nTotalLen  % MAX_DATA_SIZE == 0 ?nSeqNums : nSeqNums + 1;
    if (nTotalLen == nLen)
    {
        if (SendSeq(fd, nType, nSeqNums, 0, nLen, ptrBuff) < 0)
        {
            LOG_ERROR("send seq failed");
            return -1;
        }

        return 0;
    }

    LOG_WARNING("not surport send once upom a time");
    return -1;
}

int MsgSeqManager::SendSeq(Handle fd, int nType, int nSeqNums, int nCurrentSeq,  int nLen, const byte* ptrBuff)
{
    byte byBuff[SEQ_TYPE_SIZE];
    while (nLen > 0)
    {
        int nSendLen = nLen > MAX_DATA_SIZE ? MAX_DATA_SIZE : nLen;

        * (short*)byBuff = nSeqNums;
        * (short*) (byBuff + SEQ_NUMS_SIZE) = nCurrentSeq ++;
        * (short*) (byBuff + SEQ_NUMS_SIZE + SEQ_NUM_SIZE) = nType;
        memcpy(byBuff, ptrBuff, nSendLen + SEQ_FORMAT_SIZE);

        if (SocketHandler::SendData(fd, byBuff, nSendLen + SEQ_FORMAT_SIZE) < 0)
        {
            LOG_ERROR("send seq data failed");
            return -1;
        }

        nLen -= nSendLen;
    }
    
    return 0;
}

    int MsgSeqManager::GetDataType(RawData* ptrData)
    {
        const byte* ptrBuff = ptrData->getData();

        return * (short*)(ptrData + SEQ_NUMS_SIZE + SEQ_NUM_SIZE);
    }

    int MsgSeqManager::GetSeqNums(RawData* ptrData)
    {
        const byte* ptrBuff = ptrData->getData();

        return * (short*)(ptrData );
    }

    int MsgSeqManager::GetSeqNum(RawData* ptrData)
    {
        const byte* ptrBuff = ptrData->getData();

        return * (short*)(ptrData + SEQ_NUMS_SIZE);
    }
