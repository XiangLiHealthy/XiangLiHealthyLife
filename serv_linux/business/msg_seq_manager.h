#ifndef MSG_SEQ_MANAGER
#define MSG_SEQ_MANAGER

#include <map>
#include "../net/lib/net_data.h"
#include "../include/json/json.h"

class RawData;
using std::map;

class MsgSeqManager
{
public:
    static MsgSeqManager& GetInstance()
    {
        static MsgSeqManager singleton;
        return singleton;
    }

    int HandleSeq(RawData* ptrData);
    static int SendData(Handle fd, int nType, int nTotalLen, int nLen, const byte* ptrBuff);
private:
    map<Handle, map<short, RawData*>>m_msgSeqs;
    static map<Handle, send_sequnce_t> m_sendSeqs;
private:
    int HandleJson(RawData* ptr_data);
    int HandleFile(RawData* ptr_data);
    int HandleDisconnect(RawData* ptrData);
    int PerformWork( RawData* ptrData);

    int AddSeq(const Handle fd, const short nSeqNum, RawData* ptrData);
    RawData* GetSeq(const Handle fd, int nSeqNums);
    void RemoveSeq(const Handle fd);

    void ClearSeqs(const Handle fd);

    int GetDataType(RawData* ptrData);
    int GetSeqNums(RawData* ptrData);
    int GetSeqNum(RawData* ptrData);

    static int SendSeq(Handle fd, int nType, int nSeqNums, int nCurrentSeq,  int nLen, const byte* ptrBuff);
};

#endif