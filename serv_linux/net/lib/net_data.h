#pragma once 
typedef int Handle;
typedef unsigned char byte;

#define MAX_FRAME_SIZE 8192
#define FRAME_FORMAT_SIZE 8 

#define MAX_SEQ_SIZE MAX_FRAME_SIZE - FRAME_FORMAT_SIZE
#define SEQ_FORMAT_SIZE 6
#define MAX_DATA_SIZE MAX_SEQ_SIZE - SEQ_FORMAT_SIZE

struct send_sequnce_t
{
    short sSeqNums;
    short sSeqNum;
    short sType;
    short sWater;
    byte byBuff[MAX_SEQ_SIZE];
};

