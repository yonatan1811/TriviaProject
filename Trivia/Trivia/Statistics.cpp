#include "Statistics.h"

StatisticsManager::StatisticsManager(IDataBase* base) : m_database(base)
{
}

std::vector<string> StatisticsManager::getHighScore()
{
    return m_database->getHighScore();
}

std::vector<string> StatisticsManager::getUserStatistics(std::string username)
{
    std::vector<string> vec;
    vec.push_back(std::to_string(m_database->getNumOfCorrectAnswers(username)));
    vec.push_back(std::to_string(m_database->getNumOfPlayerGames(username)));
    vec.push_back(std::to_string(m_database->getPlayerAverageAnswerTime(username)));
    vec.push_back(std::to_string(m_database->getNumOfTotalAnswers(username)));
    return vec;
}
