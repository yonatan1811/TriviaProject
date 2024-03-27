#pragma once
#include "IDataBase.h"
#include <vector>
#include <string>


class StatisticsManager
{
public:
	StatisticsManager(IDataBase* base);
	std::vector<string> getHighScore();
	std::vector<string> getUserStatistics(std::string username);

private:
	IDataBase* m_database;
};

