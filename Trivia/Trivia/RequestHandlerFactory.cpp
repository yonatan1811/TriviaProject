#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "GameRequestHandler.h"

RequestHandlerFactory::RequestHandlerFactory(IDataBase* database) : _database(database)
{
	_database = database;
	_loginManager = new LoginManager(database);
	_statsManager = new StatisticsManager(database);
	_roomManager = new RoomManager();
	_gameManager = new GameManager(database);
}

IRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(*this);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return *_loginManager;
}

StatisticsManager& RequestHandlerFactory::getStatisticManager()
{
	return *_statsManager;
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
	return *_roomManager;
}

GameManager& RequestHandlerFactory::getGameManager()
{
	return *_gameManager;
}

IRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser& user)
{
	return new MenuRequestHandler(user, *this);
}

IRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser& user,GameData data)
{
	_gameManager->updatePlayerStats(user, data);
	return new MenuRequestHandler(user, *this);
}

IRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(Room* room, LoggedUser& user)
{
	return new RoomAdminRequestHandler(room, user, *this);
}

IRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(Room* room, LoggedUser& user)
{
	return new RoomMemberRequestHandler(room, user, *this);
}

IRequestHandler* RequestHandlerFactory::createGameRequestHandler(LoggedUser& user)
{
	return new GameRequestHandler(user, *this);
}

IRequestHandler* RequestHandlerFactory::createGameRequestHandler(Room& room, LoggedUser& user)
{
	return new GameRequestHandler(room,user, *this);
}
