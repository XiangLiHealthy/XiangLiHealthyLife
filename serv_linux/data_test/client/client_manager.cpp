#include "client_manager.h"
#include <thread>
#include <iostream>
using namespace std;

ClientManager::ClientManager(const string& strIP, const short nPort, const int nCount)
{
    m_strIP = strIP;
    m_nPort = nPort;
    m_nClientCount = nCount;
    m_bShutDown = false;
}

ClientManager::~ClientManager()
{
    for (auto itr : m_clients)
    {
        if (itr)
        {
            delete itr;
        }
    }

    m_clients.clear();
}

void Callback(void* ptrClient)
{
    Client* ptrTmp = static_cast<Client*>(ptrClient);
    ptrTmp->Start();
}

int ClientManager::StartWork()
{
    for (int index = 0; index < m_nClientCount; index ++)
    {
        //create a client
        Client* ptrClient = new Client(m_strIP, m_nPort, index, this);
        if (nullptr == ptrClient)
        {
            cout << "no memory" << endl;
            return -1;
        }
        m_clients.push_back(ptrClient);

        //start a thread
        usleep(10*1000);
        thread task(Callback, ptrClient);
        task.detach();
    }

    cout << "start client :" << m_nClientCount << endl;
    return 0;
}

int ClientManager::StopWork()
{
    m_bShutDown = true;

    for (auto itr : m_clients)
    {
        if (itr)
        {
            itr->Stop();
            delete itr;
        }
    }
    m_clients.clear();

    cout << "stop all client" << endl;
    return 0;
}

int ClientManager::SendMsg(const string& msg)
{
    m_nSentCount = 0;
    m_strMsg = msg;
    for (auto itr : m_clients)
    {
        if (itr)
        {
            itr->Notify();
        }
    }

    return 0;
}

void ClientManager::NotifySent()
{
    m_nSentCount ++;

    if (m_nSentCount == m_nClientCount)
    {
        cout << "all client send msg" << endl;
        return;
    }

    cout << "send msg : " << m_nSentCount << "/" << m_nClientCount << endl;

    return ;
}



