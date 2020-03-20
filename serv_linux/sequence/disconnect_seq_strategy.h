#ifndef DISCONNECT_SEQ_STRATEGY_
#define DISCONNECT_SEQ_STRATEGY_
#include "seq_strategy.h"

class DisconnectSeqStrategy : public SeqStrategy
{
public:
    int HandleSeq(RawData* ptrData) override;
private:

};

#endif