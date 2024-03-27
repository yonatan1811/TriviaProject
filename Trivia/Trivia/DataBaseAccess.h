#pragma once
#include "sqlite3.h"
#include "IDataBase.h"
#include <istream>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <io.h>
#include "Game.h"

struct HighScore
{
	string username;
	int score;
};

class DataBase : public IDataBase
{
public:
	bool open();
	void close();

	//queries
	bool doesUserExist(std::string name);
	bool doesPassWordExist(std::string username,std::string password);

	//adding:
	void addNewUser(std::string name , std::string pass, std::string em);

	vector<Question> getQuestions(int);
	float getPlayerAverageAnswerTime(string);
	int getNumOfCorrectAnswers(string);
	int getNumOfTotalAnswers(string);
	int getNumOfPlayerGames(string);
	int getPlayerScore(string);
	vector<string> getHighScore();
	bool addAnswerToPlayer(LoggedUser, GameData);
private:
	vector<string> getPlayers();
	sqlite3* _db;
};
