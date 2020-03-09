#include "json_seq_strategy.h"
#include "../net/lib/raw_data.h"
#include "msg_seq_manager.h"
#include "../log/log.h"

using namespace msg_seq_manager;

int JsonSeqStrategy::HandleSeq(RawData* ptrData)
{
    if (nullptr == ptrData)
    {
        LOG_ERROR("ptrData is null");
        return -1;
    }

     //weather last sequence
     int nRet = -1;
     SEQ_STATE seq_state = (SEQ_STATE)m_ptrContext->GetSeqState(ptrData);
     switch(seq_state)
     {
         case SEQ_START:
         case SEQ_BODY:
         {
            return m_ptrContext->PushSeq(ptrData);
         }break;

         case SEQ_END:
         {
             if (m_ptrContext->PushSeq(ptrData) < 0)
             {
                 LOG_ERROR("push seq error");
                 return -1;
             }
             //get merger data
             RawData* ptrTmp = m_ptrContext->FetchMergeSeq(ptrData);
             if (nullptr == ptrTmp)
             {
                 LOG_ERROR("fetch merge seq failed");
                 return -1;
             }
             
             return m_ptrContext->PerformWork(ptrTmp);
         }break;

         case SEQ_START_END:
         {
             return m_ptrContext->PerformWork(ptrData);
         }break;

         default:
         {
             LOG_ERROR("unknown seq state:%d", seq_state);
             m_ptrContext->ClearUIDSeqs(ptrData);
             delete ptrData;
         }
     }

     return nRet;
}

int JsonSeqStrategy::SendSeq(Handle fd, const Json::Value& jData)
{
    //create uid
    string strUID = MsgSeqManager::CreateSeqUID();

    string strData = jData.toStyledString();
    const char* ptrData = strData.c_str();

    int nSeqNum = MsgSeqManager::SendSeq(fd, 
    strUID, TYPE_JSON, SEQ_START_END, 0, 
   (byte*)ptrData, strData.length() );
    if (nSeqNum < 0)
   {
       LOG_ERROR("send json data failed");
       return -1;
   }

   return 0;
}