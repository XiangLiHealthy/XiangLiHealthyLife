#ifndef SEQ_STRATEGY_
#define SEQ_STRATEGY_

namespace msg_seq_manager
{
    class MsgSeqManager;
}

class RawData;
using msg_seq_manager::MsgSeqManager;

class SeqStrategy
{
public:
    void SetContext(MsgSeqManager* ptrContext)
    {
        m_ptrContext = ptrContext;
    }

    virtual int HandleSeq(RawData* ptrData) = 0;
protected:
    MsgSeqManager* m_ptrContext;
};

#endif