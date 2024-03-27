#include <iostream>
#include "Statistics.h"
#include "Requests.h"
#include "Responses.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonResponsePacketDeserializer.h"
#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;

class MenuRequestHandler : public IRequestHandler
{
public:
	bool isRequestRelevant(IRequestHandler::RequestInfo);
	IRequestHandler::RequestResult handleRequest(IRequestHandler::RequestInfo);
	MenuRequestHandler(LoggedUser&, RequestHandlerFactory&);
private:
	LoggedUser _user;
	RequestHandlerFactory _factory;

	RequestResult signout(RequestInfo);
	RequestResult getRooms(RequestInfo);
	RequestResult getPlayersInRoom(RequestInfo);
	RequestResult getPersonalStats(RequestInfo);
	RequestResult getHighScore(RequestInfo);
	RequestResult joinRoom(RequestInfo);
	RequestResult createRoom(RequestInfo);
	RequestResult logOut(RequestInfo);
};