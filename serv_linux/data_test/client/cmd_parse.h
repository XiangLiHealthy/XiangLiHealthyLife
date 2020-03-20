#ifndef CMD_PARSE_
#define CMD_PARSE_
#include "../../sequence/msg_seq_manager.h"
#include <string>

using std::string;
using msg_seq_manager::DATA_TYPE;

class CmdParse
{
public:
    DATA_TYPE GetDataType(const string& cmd);
    string GetContent(const string& cmd);
    int GetSendCount(const string& cmd);
private:

private:

};

#endif