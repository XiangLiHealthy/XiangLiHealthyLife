#include "msg_seq_manager.h"
#include "../log/log.h"
#include "../net/lib/raw_data.h"
#include "../business/proto_manager.h"
#include "../business/proto.h"
#include <string.h>
#include "../net/lib/socket_handler.h"
#include "../sequence/json_seq_strategy.h"
#include "../sequence/file_seq_strategy.h"
#include "../sequence/disconnect_seq_strategy.h"
#include <sys/time.h>
#include <thread>

static const byte SEQ_HEADER[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00};

namespace msg_seq_manager
{
    MsgSeqManager::MsgSeqManager()
    {
        m_seq_strategys[TYPE_JSON] = new JsonSeqStrategy();
        m_seq_strategys[TYPE_FILE] = new FileSeqStrategy();
        m_seq_strategys[TYPE_DISCONNECT] = new DisconnectSeqStrategy();
    }

    MsgSeqManager::~MsgSeqManager()
    {
        ClearStrategy();
        ClearAllSeqs();
    }

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
        DATA_TYPE msg_type = (DATA_TYPE)GetDataType(ptrData);
        auto itrStrategy = m_seq_strategys.find(msg_type);
        if (itrStrategy == m_seq_strategys.end())
        {
            LOG_ERROR("invalid msg type :%d", msg_type);
            delete ptrData;
            return -1;
        }

        itrStrategy->second->SetContext(this);
        return itrStrategy->second->HandleSeq(ptrData);
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

    int MsgSeqManager::GetDataType(RawData* ptrData)
    {
        const byte* ptrBuff = ptrData->getData();
        if (nullptr == ptrBuff)
        {
            LOG_ERROR("invalid para null, fd:%d", ptrData->getHandle());
            return -1;
        }

        return * (ptrBuff + UID_SIZE);
    }

    int MsgSeqManager::GetSeqNum(RawData* ptrData)
    {
        const byte* ptrBuff = ptrData->getData();
        if (nullptr == ptrBuff)
        {
            LOG_ERROR("invalid para null, fd:%d", ptrData->getHandle());
            return -1;
        }

       return * (short*)( &ptrBuff[UID_SIZE + DATA_TYPE_SIZE + SEQ_STATE_SIZE] );
    }

void MsgSeqManager::ClearAllSeqs()
{
    for (auto fd_itr: m_msgSeqs)
    {
        for (auto uid_itr: fd_itr.second)
        {
            for (auto seq_itr: uid_itr.second)
            {
                if (seq_itr)
                {
                    delete seq_itr;
                }
            }
        }
    }

    m_seq_strategys.clear();
}

void MsgSeqManager::ClearStrategy()
{
    for (auto itr: m_seq_strategys)
    {
        if (itr.second)
        {
            delete itr.second;
        }
    }

    m_seq_strategys.clear();
}

int MsgSeqManager::PushSeq(RawData* ptrData)
{
    //get handle 
    Handle fd = ptrData->getHandle();

    //get uid
    string strUID = GetSeqUID(ptrData);

    //push data
    ( (m_msgSeqs[fd])[strUID] ).push_back(ptrData);

    return 0;
}

int MsgSeqManager::SendSeq(Handle fd, 
            const string strUID, DATA_TYPE type, SEQ_STATE state, short seq_num,  
            const byte* ptrBuff, int nLen)
    {
        SEQ_STATE state_tmp = state;
        if (SEQ_START_END == state && nLen > MAX_DATA_SIZE)
        {
            state = SEQ_START;
        }        

        int nSentLen = 0;
        while (nLen > nSentLen)
        {
            int nSendLen = (nLen - nSentLen) / MAX_DATA_SIZE > 0 ? MAX_DATA_SIZE : nLen - nSentLen;

            state = (SEQ_END == state && nLen - nSentLen - nSendLen >0 ) ? SEQ_BODY : state;

            state = (SEQ_START_END == state_tmp && nLen - nSentLen - nSendLen <= 0) ? SEQ_END : state;

            send_frame_t frame_data(strUID, type, state, seq_num, ptrBuff, nSendLen);

            //send
            if (SocketHandler::SendFrame(fd, frame_data) < 0)
            {
                LOG_ERROR("send frame failed");
                return -1;
            }

            state = (SEQ_START == state) ? SEQ_BODY : state;
            seq_num ++;
            nSentLen += nSendLen;
        }

        return seq_num;
    }
     
    void MsgSeqManager::ClearUIDSeqs(RawData* ptrData)
    {
       vector<RawData*> seqs;
       if (GetSeqsByUID(ptrData, seqs) < 0)
       {
           LOG_ERROR("get seq error");
           return ;
       }

        for (auto seq_itr : seqs)
        {
            if (seq_itr)
            {
                delete seq_itr;
            }
        }

        Handle fd = ptrData->getHandle();
        string strUID = GetSeqUID(ptrData);

        m_msgSeqs[fd].erase(strUID);
    }

    void MsgSeqManager::ClearHandleSeqs(RawData* ptrData)
    {
        ClearUIDSeqs(ptrData);

        if (ptrData)
        {
            m_msgSeqs.erase(ptrData->getHandle());
        }
    }

    RawData* MsgSeqManager::FetchMergeSeq(RawData* ptrData)
    {
        if (nullptr == ptrData)
        {
            LOG_ERROR("ptrData is null");
            return nullptr;
        }

        //get total len
        int nTotalLen = CaculateSeqLen(ptrData);
        if (nTotalLen < 0)
        {
            LOG_ERROR("caculate seq len error");
            return nullptr;
        }

        //create a new RawData
        RawData* ptrTmpData = new RawData(nTotalLen + SEQ_FORMAT_SIZE);
        if (nullptr == ptrTmpData)
        {
            LOG_ERROR("no memory");
            return nullptr;
        }

        //copy data
        if (MergeSeqs(ptrTmpData, ptrData) < 0)
        {
            LOG_ERROR("merger seqs failed");
            delete ptrData;
            return nullptr;
        }

        //clear this seq
        ClearUIDSeqs(ptrData);

        return ptrTmpData;
    }

    string MsgSeqManager::CreateSeqUID()
    {
        string strUID;

        //get data time
        struct tm* now_tm = NULL;
        struct timeval now = { 0, 0 };
        gettimeofday(&now, NULL);
        time_t t = now.tv_sec;
        now_tm = localtime(&t);

        //get thread id
        pthread_t tid = pthread_self();

        char szUID[32] = {0};

        sprintf(szUID, "%04d%02d%02d%02d%02d%02d.%03ld.%08ld",
        now_tm->tm_year + 1900, now_tm->tm_mon + 1, now_tm->tm_mday,
        now_tm->tm_hour, now_tm->tm_min, now_tm->tm_sec, now.tv_usec,
        tid);

        strUID = szUID;

        return strUID;
    }

    int MsgSeqManager::CaculateSeqLen(RawData* ptrData)
    {
        if (nullptr == ptrData)
        {
            LOG_ERROR("ptrData is null");
            return -1;
        }

        vector<RawData*> seqs;
        if (GetSeqsByUID(ptrData, seqs) < 0)
        {
            LOG_ERROR("get seqs failed");
            return -1;
        }

        int nLen = 0;
        for (auto seq : seqs)
        {
            if (nullptr == seq)
            {
                LOG_ERROR("seq is nullptr");
                return -1;
            }

            nLen += (seq->getLength() - SEQ_FORMAT_SIZE);
        }

        return nLen;
    }

    int MsgSeqManager::MergeSeqs(RawData* ptrDest, const RawData* ptrSrc)
    {
        //get seqs by uid
        vector<RawData*> seqs;
        if (GetSeqsByUID((RawData*)ptrSrc, seqs) < 0)
        {
            LOG_ERROR("get seqs failed");
            return -1;
        }

        ptrDest->append(SEQ_HEADER, sizeof(SEQ_HEADER));

        //copy data but not header
        for (auto seq : seqs)
        {
            if (nullptr == seq)
            {
                LOG_ERROR("seq is null ");
                return -1;
            }

            const byte* ptrBuff = seq->getData(); 
            int nDataLen = seq->getLength() - SEQ_FORMAT_SIZE;
            ptrDest->append(ptrBuff + SEQ_FORMAT_SIZE, nDataLen);
        }

        return 0;
    }

    int MsgSeqManager::GetSeqsByUID( RawData* ptrData, vector<RawData*>& seqs)
    {
        if (nullptr == ptrData)
        {
            LOG_ERROR("ptrData is null");
            return -1;
        }

        Handle fd = ptrData->getHandle();
        auto fd_itr = m_msgSeqs.find(fd);
        if (m_msgSeqs.end() == fd_itr)
        {
            return -1;
        }

        string strUID = GetSeqUID(ptrData);
        auto uid_seqs = fd_itr->second.find(strUID);
        if (uid_seqs == fd_itr->second.end())
        {
            return -1;
        }

        seqs = uid_seqs->second;
        return 0;
    }

     int MsgSeqManager::GetSeqState( RawData* ptrData)
     {
         const byte* ptrBuff = ptrData->getData();
        return *(ptrBuff + UID_SIZE + DATA_TYPE_SIZE);
     }

     string MsgSeqManager::GetSeqUID( RawData* ptrData)
     {
         const byte* ptrBuff = ptrData->getData();
         if (nullptr == ptrBuff)
        {
            LOG_ERROR("invalid para null, fd:%d", ptrData->getHandle());
            return "";
        }

         char szUID[UID_SIZE] = {0};
         memcpy(szUID, ptrBuff, UID_SIZE -1);

         string strUID = szUID;

         return strUID;
     }
}

