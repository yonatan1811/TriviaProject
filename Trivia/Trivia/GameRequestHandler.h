#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include "IRequestHandler.h"
#include "Requests.h"
#include "Responses.h"
#include "JsonResponsePacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "LoginManager.h"
#include "RequestHandlerFactory.h"
#include "Game.h"
#include "GameManager.h"


class GameRequestHandler : public IRequestHandler
{

public:

	//constructor for existing game
	GameRequestHandler(LoggedUser& user, RequestHandlerFactory& rhf);
	//constructor for creating game
	GameRequestHandler(Room& room, LoggedUser& user, RequestHandlerFactory& rhf);
	bool isRequestRelevant(RequestInfo);
	RequestResult handleRequest(RequestInfo);
	RequestResult getQuestion(RequestInfo res);
	RequestResult submitAnswer(RequestInfo res);
	RequestResult getGameResults(RequestInfo res);
	RequestResult leaveGame(RequestInfo res);
	RequestResult hasEnded(RequestInfo res);
	RequestResult logOut(RequestInfo);
private:

	Game& _game;
	LoggedUser _user;
	RequestHandlerFactory& _factory;

};

