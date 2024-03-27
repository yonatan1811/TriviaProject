#pragma once

#include <iostream>
#include "IRequestHandler.h"
#include "Requests.h"
#include "Responses.h"
#include "JsonResponsePacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "LoginManager.h"
#include "RequestHandlerFactory.h"

class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(RequestHandlerFactory& factory);
	bool isRequestRelevant(RequestInfo info);
	IRequestHandler::RequestResult handleRequest(RequestInfo info);
private:
	IRequestHandler::RequestResult login(RequestInfo);
	IRequestHandler::RequestResult signup(RequestInfo);
	IRequestHandler::RequestResult signout(RequestInfo);
	RequestHandlerFactory _factory;
};