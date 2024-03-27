#include "GameManager.h"


GameManager::GameManager(IDataBase* _db) : _database(_db)
{
}
GameManager::GameManager(IDataBase* _db, std::vector<Game>& vec) : _database(_db), _games(vec)
{
}

std::vector<Game>& GameManager::getGames()
{
	return _games;
}

Game& GameManager::createGame(Room room)
{
	Game* game = new Game(room,_database->getQuestions(room.getData().numOfQuestions));
	_games.push_back(*game);
	return _games[_games.size()-1];
}

Game& GameManager::getGameForUser(LoggedUser& user)
{
	for (auto& gane :_games)
	{
		//find what game is the user in
		if (gane.getQuestionForUser(user).getQuestion() != "")
		{
			return gane;
		}
	}
}

void GameManager::deleteGame(int id)
{
	for (auto it = _games.begin(); it < _games.end(); it++)
	{
		if (it->getId() == id)
		{
			_games.erase(it);
		}
	}
}
void GameManager::updatePlayerStats(LoggedUser user, GameData data)
{
	_database->addAnswerToPlayer(user, data);
}