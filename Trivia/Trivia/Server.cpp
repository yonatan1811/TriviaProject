#include "Server.h"


//constructor for the server
Server::Server()
{

	// this server use TCP. that why SOCK_STREAM & IPPROTO_TCP
	// if the server use UDP we will use: SOCK_DGRAM & IPPROTO_UDP
	_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_serverSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw std::exception(__FUNCTION__ " - socket");
	}
	_database = new DataBase();
	_database->open();
	_handlerFactory = new RequestHandlerFactory(_database);
	_communicator = new Communicator(_serverSocket,SERVER_PORT,*_handlerFactory);
}

//descnostructor for the server
Server::~Server()
{
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		if (_serverSocket != INVALID_SOCKET)
		{
			closesocket(_serverSocket);
		}
	}
	catch (...) {}
}

//function for opening the server on a specific port
void Server::run(const int port)
{
	std::string input = "";
	std::thread communicatorThread(&Server::runCommunicator,this);
	communicatorThread.detach();
	do
	{
		std::cin >> input;
	} while (input != "exit" && input != "EXIT");
}

void Server::runCommunicator()
{
	_communicator->startHandleRequests();
}
