#pragma once

#include <set>
#include <thread>
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <map>
#include <queue>
#include <condition_variable>
#include "RequestHandlerFactory.h"
#include "DataBaseAccess.h"
#include "Communicator.h"

#define SERVER_PORT 2223

class Server
{
public:
	Server();
	~Server();
	//this will be the connector thread
	void run(const int port);
	void runCommunicator();

private:
	IDataBase* _database;
	SOCKET _serverSocket;
	RequestHandlerFactory* _handlerFactory;
	std::mutex _userLock;
	std::mutex _msgLock;
	std::condition_variable _conditionVar;
	std::set<std::string> _files;
	std::map<int, SOCKET> _clients;
	std::set<std::string>_msgQueue;
	Communicator* _communicator;
	std::vector<std::thread> _threads;
};	

	

