#pragma once
#include "IDataBase.h"
#include "Game.h"
#include "Room.h"

class GameManager
{
public:
	GameManager(IDataBase*);
	GameManager(IDataBase*, std::vector<Game>&);
	Game& createGame(Room);
	Game& getGameForUser(LoggedUser& user);
	void deleteGame(int);
	std::vector<Game>& getGames();
	void updatePlayerStats(LoggedUser user, GameData data);
private:
	IDataBase* _database;
	std::vector<Game> _games;

};


