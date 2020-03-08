#ifndef CLIENT_MANAGER_
#define CLIENT_MANAGER_

#include "client.h"
#include <vector>
using std::vector;

class ClientManager
{
public:
    ClientManager(const string& strIP, const short nPort, const int nCount);
    ~ClientManager();

    int StartWork();
    int StopWork();
    int SendMsg( const string& msg);

    void NotifySent();
public:
    bool m_bShutDown;
    string m_strMsg;
    volatile int m_nSentCount;
private:
    vector<Client*> m_clients;
    string m_strIP;
    short m_nPort;
    int m_nClientCount;
private:
    
};

#endif