#include<stdio.h>
#include <iostream>
#include <string.h>
#include "client_manager.h"

using namespace std;
int ParseCmd(int argc, char* argv[], string& strIP, short& nPort, int& nCount);

int main(int argc, char* argv[]) 
{
	string strIP;
	short nPort;
	int nClientCount;
	if (ParseCmd(argc, argv, strIP, nPort, nClientCount) < 0)
	{
		cout << "parse cmd failed:" << argc << endl;
		return 0;
	}

	ClientManager clients(strIP, nPort, nClientCount);

	if (clients.StartWork() < 0)
	{
		cout << "start client mananger failed" << endl;
		return 0;
	}

	string msg = "{\"proto\": \"Clinics\"}";

	while (getline(cin, msg))
	//while(true)
	{
		cout << "input:" << msg << endl;
		
		if (clients.SendMsg(msg) < 0)
		{
			cout << "msg send failed:" << msg << endl;
		}

		//sleep(1);
	}

	clients.StopWork();

	cout << "stop work" << endl;
	
	return 0;
}

int ParseCmd(int argc, char* argv[], string& strIP, short& nPort, int& nCount)
{
	if (argc != 4)
	{
		cout << "you should input 3 parameters,such as:127.0.0.1 6666 1" << endl;
		return -1;
	}

	strIP = argv[1];

	sscanf(argv[2], "%d", &nPort);
	sscanf(argv[3], "%d", &nCount);
	// nPort = strtol(argv[1], argv[1] + strlen(argv[1], 10);
	// nCount = strtol(argv[2]);

	return 0;
}




















