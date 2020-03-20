#ifndef CLIENT_
#define CLIENT_

#include<stdio.h>
#include"sys/socket.h"
#include"arpa/inet.h"
#include<iostream>
#include<errno.h>
#include<string.h>
#include"unistd.h"
#include "../../net/lib/socket_handler.h"
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/epoll.h>


class ClientManager;

class Client
{
public:
    Client(const string& strIP, short nPort, int nNum, ClientManager* ptrContext);
    ~Client();

    int Start();
    void Stop();

    void Notify();
private:
    int ConnectServ();
    int RecvMsg();
    int SendMsg();
    int PrepareEpoll();
private:
    int m_fd;
    int m_epoll;
    int m_pipe[2];
    struct epoll_event ev;


    string m_strIP;
    short m_nPort;
    int m_nNum;
    volatile bool m_bSent;
    ClientManager* m_ptrContext;
    SocketHandler* m_ptrRecver;
};

#endif