#ifndef JSON_SEQ_STRTEGY_
#define JSON_SEQ_STRTEGY_

#include "seq_strategy.h"
#include "../net/lib/net_data.h"
#include "../../include/json/json.h"

class JsonSeqStrategy : public SeqStrategy
{
public:
    int HandleSeq(RawData* ptrData) override;
    
    static int SendSeq(Handle fd, const Json::Value& jData);
private:
    int PushSeq(RawData* ptrData);
    int PerformWork(RawData* ptrData);
};

#endif