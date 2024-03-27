#pragma once
#include <vector>
#include "JsonResponsePacketDeserializer.h"
#include <ctime>

class IRequestHandler
{
public:
	struct RequestInfo
	{
		int id;
		int recivelTime;
		std::vector<unsigned char> buffer;
	};

	struct RequestResult
	{
		std::vector<unsigned char> buffer;
		IRequestHandler* newHandler;
	};
	virtual bool isRequestRelevant(RequestInfo) = 0; 
	virtual RequestResult handleRequest(RequestInfo) = 0;
private:

};
