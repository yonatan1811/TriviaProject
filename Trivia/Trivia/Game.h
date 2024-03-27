#pragma once
#include "Question.h"
#include <map>
#include "LoggedUser.h"
#include "Room.h"
#include "Responses.h"
#include <unordered_map>


struct GameData
{
	GameData(void) : currentQuetion(Question("", "", "", "", "")) , correctAnswerCount(0) , wrongAnswerCount(0) , averegeAnswerTime(0)
	{
	}
	Question currentQuetion;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averegeAnswerTime;
	bool operator < (const GameData& GD) const
	{
		if (this->correctAnswerCount < GD.correctAnswerCount)
		{
			return false;
		}
		if (this->correctAnswerCount > GD.correctAnswerCount)
		{
			return true;
		}
		return !(this->averegeAnswerTime < GD.averegeAnswerTime);
	}

};



class Game
{
public:
	Game();
	Game(Room room, std::vector<Question> questions);
	Question getQuestionForUser(LoggedUser);
	bool submitAnswer(LoggedUser, int, int);
	void RemovePlayer(LoggedUser);
	int getId();
	std::vector<PlayerResult> getResults();
	bool hasEnded();
	GameData getUserData(LoggedUser user);
private:
	std::vector<Question> _questions;
	std::map<LoggedUser, GameData> _players;
	unsigned int _gameId;
	RoomData _data;
	int _playersFinished;
};


