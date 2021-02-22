#include "client.h"
#include <iostream>
#include "client_manager.h"
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include "../../../include/json/json.h"
#include "../../sequence/msg_seq_manager.h"
#include "../../sequence/json_seq_strategy.h"
#include "../../log/log.h"

 #define SHUTDOWN 0
 #define SEND_MSG 1

using namespace std;

Client::Client(const string& strIP, short nPort, int nNum, ClientManager* ptrContext)
{
    m_bSent = true;;
    m_fd = -1;
    m_epoll = -1;
    m_nNum = nNum;
    m_nPort = nPort;
    m_strIP = strIP;
    m_pipe[0]  = -1;
    m_pipe[1] = -1;
    m_ptrContext = ptrContext;
}

Client::~Client()
{
    Stop();
}

int Client::Start()
{
    cout << "client :" << m_nNum << "start task" << endl; 
    //connect service
    if (ConnectServ() < 0)
    {
        cout << "client num:" << m_nNum << "connect serv failed" << endl;
        return -1;
    }

    if (PrepareEpoll() < 0)
    {
        cout << "client :" << m_nNum << "prepare epoll failed" << endl;
        return -1;
    }

     struct epoll_event events[16];

    //perform task
    cout << "client :" << m_nNum << "perform work" << endl;
    while ( !m_ptrContext->m_bShutDown)
    {
        //epoll wait
        int nums = epoll_wait(m_epoll, events, sizeof(events)/sizeof(events[0]),100000);

        if (nums < 0)
        {
            cout << "epoll error:" << strerror(errno) << endl;
        }
        else
        {
            cout << m_nNum << "epoll trigger:" << nums << endl;
        }
        
        

        for (int index = 0; index < nums; index ++)
        {
            int nCurrentFd = events[index].data.fd;

            cout << "fd:" << nCurrentFd << ", ev:" << events[index].events << endl;
            cout << "m_fd:" << m_fd << ", pipe[0]" << m_pipe[0] << ", pipe[1]:" << m_pipe[1] << endl;

            if (nCurrentFd == m_fd && EPOLLIN & events[index].events)
            {
                     //there is service msg,  recv msg
                     if (RecvMsg() < 0)
                     {
                         cout << "client :" << m_nNum << "recv msg failed" << endl;
                     }
            }
            else if (nCurrentFd == m_pipe[0] && EPOLLIN & events[index].events)
            {
                cout << "client :" << m_nNum << "send a msg" << endl;
                
                char szBuff[8] = {0};
                if (read(m_pipe[0], szBuff, sizeof(szBuff)) <= 0)
                {
                    cout << "client :" << m_nNum << "read data failed:" << strerror(errno) << endl;
                    continue;
                }

                if (szBuff[0] == SHUTDOWN)
                {
                    break;
                }

                //send msg
                if (SendMsg() < 0)
                {
                    cout << "client :" << m_nNum << "send msg failed" << endl;
                }
            }
        }
    }

    cout << "client:" << m_nNum << "stop task" << endl;
    return 0;
}

void Client::Stop()
{
    if ( m_fd < 0)
    {
        close(m_fd);
        m_fd = -1;
    }

    if (m_pipe[0] < 0)
    {
        byte byCmd = SHUTDOWN;
        if (write(m_pipe[1], &byCmd, sizeof(byCmd)) < sizeof(byCmd))
        {
            cout << "client:" << m_nNum << "stop failed" << endl;
        }

        close (m_pipe[0]);
        close (m_pipe[1]);
    }

    if ( m_epoll < 0)
    {
        close(m_epoll);
        m_epoll = -1;
    }

}

int Client::ConnectServ()
{
    int fd = -1;
	if ( (fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		std::cerr << "socket error " << errno <<std::endl;
		return -1;
	}

	struct sockaddr_in seraddr;
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(m_nPort);
	seraddr.sin_addr.s_addr = inet_addr(m_strIP.c_str());

    cout << "connect ser ip:" << m_strIP << ", port:" << m_nPort << endl;
	if (connect (fd, (struct sockaddr*)&seraddr, sizeof(seraddr)) < 0)
	{
		std::cerr << "connect error" << errno << std::endl;
		return -1;
	}

   if (pipe(m_pipe) < 0)
   {
       cout << "pipe create error:" << strerror(errno) << endl;
       return -1;
   }
    
    m_ptrRecver = new SocketHandler(fd);
    if (nullptr == m_ptrRecver)
    {
        cout << "client:" << m_nNum << "no memory" << endl;
        return -1;
    }

    m_fd = fd;
    return 0;
}

int Client::RecvMsg()
{
    m_ptrRecver->handle_read();
    return 0;
}

int Client::SendMsg()
{
    cout << "clent send msg :" << m_nNum <<endl;

    Json::Value jData;
    Json::Reader reader;

    //chang msg into json
    if (reader.parse(m_ptrContext->m_strMsg, jData) < 0)
    {
        cout << "client:" << m_nNum << "parse json failed:" << m_ptrContext->m_strMsg << endl;
        return -1;
    }

    //send msg
    int nRet = JsonSeqStrategy::SendSeq(m_fd, jData);

     m_ptrContext->NotifySent();

     return nRet;
}


void Client::Notify()
{
    cout << "notify:" << m_nNum << endl;

    m_bSent = false;

    //write a byte to pipe
    byte byCmd = SEND_MSG;
    if (write(m_pipe[1], &byCmd, sizeof(byCmd) ) < sizeof(byCmd))
    {
        cout << "client :" << m_nNum << "notify failed:" << strerror(errno) << endl;
    }
}

int Client::PrepareEpoll()
{
    // 创建epoll树
    int epfd = epoll_create(1024);
    if(epfd == -1)
    {
        perror("epoll_create");
       return -1;
    }

    m_epoll = epfd;

    // 将监听client添加到树上
    struct epoll_event ev;

    ev.events = EPOLLIN  ;
    ev.data.fd = m_fd;
    cout << "client fd:" << m_fd << endl;

    if ( epoll_ctl(m_epoll, EPOLL_CTL_ADD, m_fd, &ev) < 0)
    {
        cout << "add client fd error:" << strerror(errno) << endl;
        return -1;
    }

    //将pipe添加到树上
    ev.events = EPOLLIN ;
    ev.data.fd = m_pipe[0];
    cout <<  "pipe[0]:" << m_pipe[0] << ", pipe[1]:" << m_pipe[1] << endl;

    if (epoll_ctl(m_epoll, EPOLL_CTL_ADD, m_pipe[0], &ev) < 0)
    {
        cout << "add pipe fd error:" << strerror(errno) << endl;
        return -1;
    }

    return 0;
}