#include "test.h"
#include "../sequence/json_seq_strategy.h"
#include "../log/log.h"

Test::Test()
{
    strcpy(m_szName, "test");
}

const char* Test::getName()
{
    return m_szName;
}

int Test::dispatch(const Json::Value& jData, Handle fd)
{
    if (JsonSeqStrategy::SendSeq( fd, jData) < 0)
    {
        LOG_ERROR("send json failed");
        return -1;
    }

    return 0;
}