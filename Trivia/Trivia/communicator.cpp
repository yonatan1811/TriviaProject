#include "Communicator.h"

Communicator::Communicator(const SOCKET serverSocket, const int serverPort,RequestHandlerFactory& handlerFactory) : _serverSocket(serverSocket) , _serverPort(serverPort) , _HandlerFactory(handlerFactory)
{
}

void Communicator::startHandleRequests()
{
	bindAndListen();
}

SOCKET Communicator::acceptClient()
{
	// this accepts the client and create a specific socket from server to this client
	// the process will not continue until a client connects to the server
	SOCKET client_socket = accept(_serverSocket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);

	std::cout << "Client accepted. Server and client can speak" << std::endl;
	std::thread currClientThread(&Communicator::HandleNewClient, this, client_socket);
	currClientThread.detach();
	return client_socket;
}

void Communicator::HandleNewClient(SOCKET userSock)
{
	try
	{
		_clients.insert(std::pair<SOCKET, IRequestHandler*>(userSock, _HandlerFactory.createLoginRequestHandler()));
		continueConversation(userSock);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void Communicator::continueConversation(SOCKET userSock)
{
	while (true)
	{
		if (_clients.at(userSock) == nullptr)
		{
			break;
		}
		std::string dataFromUser = "";
		int code = JsonRequestPacketDeserializer::binaryToDecInt(Helper::getStringPartFromSocket(userSock, 1 * BYTE_LEN));
		int len = JsonRequestPacketDeserializer::binaryToDecInt(Helper::getStringPartFromSocket(userSock, 4 * BYTE_LEN));
		if(len > 0 && len < 1024)
			dataFromUser = JsonRequestPacketDeserializer::BinToString(Helper::getStringPartFromSocket(userSock, len * BYTE_LEN));

		std::cout <<  "got code :: " << code << std::endl;
		std::cout << len << std::endl;
		std::cout << dataFromUser << std::endl;

		std::vector<unsigned char> vec;
		for (int i = 0; i < len/8; i++)
		{
			vec.push_back(dataFromUser[i]);
		}

		IRequestHandler::RequestInfo currRequest;
		currRequest.id = code;
		currRequest.recivelTime = time(0);
		currRequest.buffer = vec;

		IRequestHandler::RequestResult result = _clients.at(userSock)->handleRequest(currRequest);
		_clients.at(userSock) = result.newHandler;
		std::string finalMsg = "";
		for (unsigned char c : result.buffer)
		{
			finalMsg += c;
		}

		Helper::sendData(userSock, finalMsg);
	}
	std::cout << "bye bye user !!" << std::endl;
	_clients.erase(userSock);
}

void Communicator::bindAndListen()
{
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(_serverPort); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// Connects between the socket and the configuration (port and etc..)
	if (bind(_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << _serverPort << std::endl;

	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		std::cout << "Waiting for client connection request" << std::endl;
		acceptClient();
	}
}


