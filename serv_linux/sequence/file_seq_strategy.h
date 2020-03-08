#ifndef FILE_SEQ_STRATEGY_
#define FILE_SEQ_STRATEGY_

#include "seq_strategy.h"

class FileSeqStrategy : public SeqStrategy
{
public:
    int HandleSeq(RawData* ptrData) override;
private:

};

#endif
