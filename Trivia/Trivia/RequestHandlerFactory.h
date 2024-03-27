#pragma once

#include <iostream>
#include "IDataBase.h"
#include "LoginManager.h"
#include "RoomManager.h"
#include "Statistics.h"
#include "IRequestHandler.h"
#include "GameManager.h"

class RequestHandlerFactory
{
private:
	IDataBase* _database;
	LoginManager* _loginManager;
	RoomManager* _roomManager;
	StatisticsManager* _statsManager;
	GameManager* _gameManager;
public:
	RequestHandlerFactory(IDataBase*);
	IRequestHandler* createLoginRequestHandler();
	LoginManager& getLoginManager();
	IRequestHandler* createMenuRequestHandler(LoggedUser&);
	IRequestHandler* createMenuRequestHandler(LoggedUser& user, GameData data);
	StatisticsManager& getStatisticManager();
	RoomManager& getRoomManager();
	GameManager& getGameManager();
	IRequestHandler* createRoomAdminRequestHandler(Room*, LoggedUser&);
	IRequestHandler* createRoomMemberRequestHandler(Room*, LoggedUser&);
	IRequestHandler* createGameRequestHandler(Room&,LoggedUser&);
	IRequestHandler* createGameRequestHandler(LoggedUser&);

};