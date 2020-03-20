#pragma once 
typedef int Handle;
typedef unsigned char byte;

#define MAX_FRAME_SIZE 8192
#define FRAME_FORMAT_SIZE 8 

#define MAX_SEQ_SIZE MAX_FRAME_SIZE - FRAME_FORMAT_SIZE
#define UID_SIZE 32
#define DATA_TYPE_SIZE 1
#define SEQ_STATE_SIZE 1
#define SEQ_NUM_SIZE 2
#define SEQ_FORMAT_SIZE UID_SIZE + DATA_TYPE_SIZE + SEQ_STATE_SIZE + SEQ_NUM_SIZE
#define MAX_DATA_SIZE MAX_SEQ_SIZE - SEQ_FORMAT_SIZE

#include<string.h>
#include <string>
using std::string;

struct send_frame_t
{
    byte* ptrFrameHeader;
    short* ptrCheckSum;
    short* ptrSeqLen;
    char* ptrUID;
    byte* ptrDataType;
    byte* ptrSeqState;
    short* ptrSeqNum;
    byte* ptrData;
    byte* ptrFrameTail;
    byte byBuff[MAX_FRAME_SIZE];
    short nFrameLen;

    send_frame_t(const string& strUID, byte type, byte state, short seq_num, const byte* ptrBuff, int nLen )
    {
        ptrFrameHeader = byBuff;
        ptrCheckSum = (short*) ((byte*)byBuff + 2);
        ptrSeqLen = (short*) ((byte*)byBuff + 4);
        ptrUID = (char*) ((byte*)byBuff + 6);
        ptrDataType = (byte*)byBuff + 6 + 32;
        ptrSeqState = (byte*)byBuff + 6 + 32 + 1;
        ptrSeqNum = (short*) ((byte*)byBuff + 40);
        ptrData = (byte*)byBuff + 42;

         const char* szUID = strUID.c_str();
        memcpy(ptrUID, szUID, strUID.length());
        *ptrDataType = type;
        *ptrSeqState = state;
        *ptrSeqNum = seq_num;

        if (ptrBuff)
        {
            memcpy(ptrData, ptrBuff, nLen);
        }
        else
        {
            nLen = 0;
        }
        
    
        ptrFrameTail = ptrData + nLen;

        nFrameLen = nLen + FRAME_FORMAT_SIZE + SEQ_FORMAT_SIZE;
        *ptrSeqLen = nLen + SEQ_FORMAT_SIZE;
    }
};

