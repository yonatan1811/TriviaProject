#pragma once
#include <iostream>
#include "IRequestHandler.h"
#include "Requests.h"
#include "Responses.h"
#include "Room.h"
#include "LoggedUser.h"
#include "JsonResponsePacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "RequestHandlerFactory.h"

class RoomAdminRequestHandler : public IRequestHandler
{
public:
	RoomAdminRequestHandler(Room* room, LoggedUser& user,RequestHandlerFactory& factory);
	bool isRequestRelevant(RequestInfo info);
	IRequestHandler::RequestResult handleRequest(RequestInfo info);
private:
	Room* _room;
	LoggedUser _user;
	RequestHandlerFactory _factory;

	IRequestHandler::RequestResult closeRoom(IRequestHandler::RequestInfo);
	IRequestHandler::RequestResult startGame(IRequestHandler::RequestInfo);
	IRequestHandler::RequestResult getRoomState(IRequestHandler::RequestInfo);
	IRequestHandler::RequestResult logOut(RequestInfo);
};