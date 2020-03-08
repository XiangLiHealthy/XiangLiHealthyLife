#include "disconnect_seq_strategy.h"
#include "../net/lib/raw_data.h"
#include "msg_seq_manager.h"
#include "../log/log.h"

int DisconnectSeqStrategy::HandleSeq(RawData* ptrData)
{
    LOG_INFO("disconnet");
    m_ptrContext->ClearHandleSeqs(ptrData);

    return 0;
}