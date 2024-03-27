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


class RoomMemberRequestHandler : public IRequestHandler
{
public:

	RoomMemberRequestHandler(Room* room, LoggedUser& user, RequestHandlerFactory& handle);
	bool isRequestRelevant(RequestInfo) override;
	RequestResult handleRequest(RequestInfo) override;
	RequestResult leaveRoom(RequestInfo);
	RequestResult getRoomState(RequestInfo);
	RequestResult logOut(RequestInfo);
private:

	Room* _room;
	LoggedUser _user;
	RequestHandlerFactory _factory;

};

