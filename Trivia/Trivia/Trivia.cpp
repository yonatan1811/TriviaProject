// Trivia.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Server.h"
#include "WSAInitializer.h"
#include <WinSock2.h>


#define ERROR_CODE -1

int main()
{
    std::cout << "Hello World!\n";
	WSAInitializer wsaInit;
	Server s;
	try
	{
		s.run(SERVER_PORT);
	}
	catch (const std::exception& e )
	{
		std::cout << e.what() << std::endl;
		return ERROR_CODE;
	}
	return 0;
}

