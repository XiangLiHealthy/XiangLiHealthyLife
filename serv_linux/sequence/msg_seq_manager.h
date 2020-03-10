#ifndef MSG_SEQ_MANAGER
#define MSG_SEQ_MANAGER

#include <map>
#include "../net/lib/net_data.h"
#include "../include/json/json.h"

class RawData;
using std::map;
using std::vector;
class SeqStrategy;

namespace msg_seq_manager
{
    enum DATA_TYPE 
    {
        TYPE_JSON = 1,
        TYPE_FILE = 2,
        TYPE_DISCONNECT = 3,
    };

    enum SEQ_STATE
    {
        SEQ_START = 0,
        SEQ_BODY = 1,
        SEQ_END = 2,
        SEQ_START_END,
    };


    class MsgSeqManager
    {
    public:
        static MsgSeqManager& GetInstance()
        {
            static MsgSeqManager singleton;
            return singleton;
        }

        int HandleSeq(RawData* ptrData);
      
        
        int GetSeqNums(RawData* ptrData);
        int GetSeqNum(RawData* ptrData);
        int GetSeqState( RawData* ptrData);
        string GetSeqUID( RawData* ptrData);

         int PerformWork( RawData* ptrData);
         int PushSeq(RawData* ptrData);
        void ClearUIDSeqs(RawData* ptrData);
        void ClearHandleSeqs(RawData* ptrData);
        RawData* FetchMergeSeq(RawData* ptrData);

         static int SendSeq(Handle fd, 
               const string strUID, DATA_TYPE type, SEQ_STATE state, short seq_num,  
                const byte* ptrBuff, int nLen);
        static string CreateSeqUID();
    private:
        map<Handle, map<string, vector<RawData*>>>m_msgSeqs;
        map<DATA_TYPE, SeqStrategy*> m_seq_strategys;

    private:
        MsgSeqManager();
        ~MsgSeqManager();

        void ClearStrategy();
        void ClearAllSeqs();

        int GetDataType(RawData* ptrData);
        int CaculateSeqLen(RawData* ptrData);  
        int MergeSeqs(RawData* ptrDest, const RawData* ptrSrc);
        int GetSeqsByUID( RawData* ptrData, vector<RawData*>& seqs);
        const byte* GetDataContent(RawData* ptrData);
    };
};



#endif