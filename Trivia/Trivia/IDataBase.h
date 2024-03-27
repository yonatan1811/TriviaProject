#pragma once
#include <string>
#include <list>
#include <vector>
#include "question.h";
#include "Game.h"
using std::string;
using std::vector;


class IDataBase
{
public:
	
	//on the ram
	virtual bool open() = 0;
	virtual void close() = 0;
	virtual bool doesUserExist(string) = 0;
	virtual bool doesPassWordExist(string, string) = 0;
	virtual void addNewUser(string, string, string) = 0;

	//on the db
	virtual vector<Question> getQuestions(int) = 0;
	virtual float getPlayerAverageAnswerTime(string) = 0;
	virtual int getNumOfCorrectAnswers(string) = 0;
	virtual int getNumOfTotalAnswers(string) = 0;
	virtual int getNumOfPlayerGames(string) = 0;
	virtual int getPlayerScore(string) = 0;
	virtual vector<string> getHighScore() = 0;
	virtual bool addAnswerToPlayer(LoggedUser, GameData) = 0;
private:

};
