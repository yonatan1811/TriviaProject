#pragma once

#include "Requests.h"
#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
#include "JsonResponsePacketDeserializer.h"
#include "RequestHandlerFactory.h"
#include "Helper.h"
#include <thread>

#define MSG_LEN 1024

class Communicator
{
public:
	Communicator(const SOCKET serverSocket, const int serverPort,RequestHandlerFactory& handlerFactory);

	void startHandleRequests();
	

private:
	SOCKET acceptClient();
	void bindAndListen();
	void HandleNewClient(SOCKET userSock);
	void continueConversation(SOCKET userSock);

	RequestHandlerFactory _HandlerFactory;
	std::map<SOCKET, IRequestHandler*> _clients;
	SOCKET _serverSocket;
	int _serverPort;
};