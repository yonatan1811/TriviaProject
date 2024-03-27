#include "DataBaseAccess.h"
#include <algorithm>



int callBackUserName(void* data, int argc, char** argv, char** azColName)
{
	std::string* name = (std::string*)(data);
	*name = argv[0];
	return 0;
}

int callBackCountUser(void* data, int agrc, char** argv, char** azColName)
{
	int* id = (int*)(data);
	*id = std::stoi(argv[0]);
	return 0;
}


int callBackAvgTime(void* data, int agrc, char** argv, char** azColName)
{
	float* avg = (float*)data;
	*avg = std::stof(argv[0]);
	return 0;
}

int callBackGetPlayers(void* data, int agrc, char** argv, char** azColName)
{
	vector<string>* vec = (vector<string>*)data;
	vec->push_back(argv[0]);
	return 0;
}

int callBackGetQuestions(void* data, int agrc, char** argv, char** azColName)
{
	vector<Question>* vec = (vector<Question>*)data;
	string question = argv[0];
	string corr = argv[1];
	string wrong1 = argv[2];
	string wrong2 = argv[3];
	string wrong3 = argv[4];
	Question* q = new Question(question, corr, wrong1, wrong2, wrong3);
	vec->push_back(*q);
	return 0;
}

bool DataBase::open()
{
	std::string dbFileName = "MyDB1.1.sqlite"; // creating a new db 
	int file_exist = _access(dbFileName.c_str(), 0);
	int res = sqlite3_open(dbFileName.c_str(), &_db);
	if (res != SQLITE_OK) {
		_db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return false;
	}

	//creating the table
	char* errMessage = nullptr;
	const char* sqlStatement = "CREATE TABLE IF NOT EXISTS USERS (NAME TEXT PRIMARY KEY NOT NULL, PASSWORD TEXT NOT NULL , MAIL TEXT NOT NULL);";
	errMessage = nullptr;
	res = sqlite3_exec(_db, sqlStatement, nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
	{
		_db = nullptr;
		throw("Error creating table");
	}



	sqlStatement = "CREATE TABLE IF NOT EXISTS QUESTIONS (question TEXT NOT NULL, correct TEXT NOT NULL, wrong1 TEXT NOT NULL, wrong2 TEXT NOT NULL, wrong3 TEXT NOT NULL);";
	res = sqlite3_exec(_db, sqlStatement, nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
	{
		_db = nullptr;
		throw("Error creating table");
	}


	sqlStatement = "CREATE TABLE IF NOT EXISTS STATISTICS (USERNAME TEXT PRIMARY KEY NOT NULL , RIGHTGUESSES INT NOT NULL , AVGTIME REAL , TOTALANSWERS INT , NUMBEROFGAMES INT);";
	res = sqlite3_exec(_db, sqlStatement, nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
	{
		_db = nullptr;
		throw("Error creating table");
	}
	return true;
}

bool DataBase::doesUserExist(std::string name)
{
	std::string sqlState = "SELECT COUNT(1) FROM USERS WHERE NAME = '" + name + "';";
	char* errMessage = nullptr;
	int count = 0;
	int res = sqlite3_exec(_db, sqlState.c_str(), callBackCountUser, &count, &errMessage);
	if (count != 0)
	{
		return true;
	}
	return false;
}

bool DataBase::doesPassWordExist(std::string username, std::string password)
{
	std::string sqlState = "SELECT COUNT(1) FROM USERS WHERE NAME = '"+ username + "' AND PASSWORD = '" + password + "';";
	char* errMessage = nullptr;
	int count = 0;
	int res = sqlite3_exec(_db, sqlState.c_str(), callBackCountUser, &count, &errMessage); //same function
	if (count != 0)
	{
		return true;
	}
	return false;
}

void DataBase::addNewUser(std::string name, std::string pass, std::string em)
{
	char* errMessage = nullptr;
	try
	{
		std::string sql = "INSERT OR IGNORE INTO USERS VALUES('" + name + "','" + pass + "','" + em + "');";
		int res = 0;
		errMessage = nullptr;
		res = sqlite3_exec(_db, sql.c_str(), NULL, NULL, &errMessage);

		sql = "INSERT OR IGNORE INTO STATISTICS VALUES('" + name + "',0,0.1,0,0);";
		res = 0;
		errMessage = nullptr;
		res = sqlite3_exec(_db, sql.c_str(), NULL, NULL, &errMessage);
	}
	catch (std::exception)
	{
		throw errMessage;
	}

}




float DataBase::getPlayerAverageAnswerTime(string d )
{
	std::string sql = "SELECT AVGTIME FROM STATISTICS WHERE USERNAME = '" +d + "';";
	int res = 0;
	float avg = 0;;
	char* errMessage = nullptr;
	res = sqlite3_exec(_db, sql.c_str(), callBackAvgTime, &avg, &errMessage);
	return avg;
}
int DataBase::getNumOfCorrectAnswers(string name)
{
	std::string sqlState = "SELECT RIGHTGUESSES FROM STATISTICS WHERE USERNAME = '" + name + "';";
	char* errMessage = nullptr;
	int count = 0;
	int res = sqlite3_exec(_db, sqlState.c_str(), callBackCountUser, &count, &errMessage); //same function
	return count;
}
int DataBase::getNumOfTotalAnswers(string name)
{
	std::string sqlState = "SELECT TOTALANSWERS FROM STATISTICS WHERE USERNAME = '" + name + "';";
	char* errMessage = nullptr;
	int count = 0;
	int res = sqlite3_exec(_db, sqlState.c_str(), callBackCountUser, &count, &errMessage); //same function
	return count;
}
int DataBase::getNumOfPlayerGames(string name)
{
	std::string sqlState = "SELECT NUMBEROFGAMES FROM STATISTICS WHERE USERNAME = '" + name + "';";
	char* errMessage = nullptr;
	int count = 0;
	int res = sqlite3_exec(_db, sqlState.c_str(), callBackCountUser, &count, &errMessage); //same function
	return count;
}
int DataBase::getPlayerScore(string name)
{
	if (getNumOfTotalAnswers(name) == 0)
	{
		return 0;
	}
	float res = (float)getNumOfCorrectAnswers(name) / (float)getNumOfTotalAnswers(name);
	return res * 100;
}

bool cmp(HighScore h1, HighScore h2)
{
	return h1.score > h2.score;
}

vector<string> DataBase::getPlayers()
{
	vector<string> players;
	std::string sqlState = "SELECT USERNAME FROM STATISTICS;";
	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, sqlState.c_str(), callBackGetPlayers, &players, &errMessage);
	return players;
}

vector<string> DataBase::getHighScore()
{
	vector<string> players = getPlayers();
	vector<HighScore> scores;
	vector<string> sortedScores;
	for (string name : players)
	{
		scores.push_back({ name , getPlayerScore(name) });
	}
	std::sort(scores.begin(), scores.end(), cmp);
	for (HighScore score : scores)
	{
		sortedScores.push_back(score.username);
		sortedScores.push_back(std::to_string(score.score));
	}
	return sortedScores;
}

bool DataBase::addAnswerToPlayer(LoggedUser user,GameData data)
{
	if (data.averegeAnswerTime == 0)
	{
		data.averegeAnswerTime = 0.1;
	}
	float oldAvg = getPlayerAverageAnswerTime(user.getUsername());
	float newAvg = ((getNumOfTotalAnswers(user.getUsername())) * oldAvg + data.averegeAnswerTime) / (data.wrongAnswerCount + data.correctAnswerCount + getNumOfTotalAnswers(user.getUsername()));
	if (newAvg == 0)
	{
		newAvg = 0.1;
	}

	string sqlStatement = "UPDATE STATISTICS SET AVGTIME = " + std::to_string(newAvg) + ", RIGHTGUESSES = RIGHTGUESSES + " + std::to_string(data.correctAnswerCount) +
		", TOTALANSWERS = TOTALANSWERS + " + std::to_string(data.wrongAnswerCount + data.correctAnswerCount) + ", NUMBEROFGAMES = NUMBEROFGAMES + 1" + " WHERE USERNAME = '" + user.getUsername() + "';";

	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, sqlStatement.c_str(), nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		throw errMessage;
	return true;
}

vector<Question> DataBase::getQuestions(int num)
{
	vector<Question> questions;
	std::string sqlState = "SELECT * FROM QUESTIONS ORDER BY RANDOM() LIMIT " + std::to_string(num) + "; ";
	char* errMessage = nullptr;
	int res = sqlite3_exec(_db, sqlState.c_str(), callBackGetQuestions, &questions, &errMessage);
	std::cout << "THERE WERE " + std::to_string(num) + " QUESTIONS ASKED GOT: " + std::to_string(questions.size()) + "\n";
	return questions;
}

void DataBase::close()
{
	sqlite3_close(_db);
	_db = nullptr;
}
