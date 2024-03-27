#include "Game.h"

Game::Game(Room room, std::vector<Question> questions) : _questions(questions)
{
    _playersFinished = 0;
    _data = room.getData();
    _gameId = _data.id;
    std::vector<LoggedUser> players = room.getLoggedUsers();
    for (auto player : players)
    {
        GameData gameData;
        gameData.averegeAnswerTime = 0;
        gameData.correctAnswerCount = 0;
        gameData.wrongAnswerCount = 0;
        gameData.currentQuetion = _questions[0];
        _players.insert({ player,gameData });
    }
}

Question Game::getQuestionForUser(LoggedUser user)
{
    //debug::

    if ((_players.find(user) != _players.end() )&& _players[user].correctAnswerCount + _players[user].wrongAnswerCount < _data.numOfQuestions)
    {
        return _players[user].currentQuetion;
    }

    _playersFinished++;
    return Question("ERROR", "ERROR", "ERROR", "ERROR", "ERROR");
}

GameData Game::getUserData(LoggedUser user)
{
    if (_players.find(user) != _players.end())
    {
        return _players.at(user);
    }
    GameData a;
    a.averegeAnswerTime = -1;
    a.correctAnswerCount = -1;
    a.wrongAnswerCount = -1;
    return a;
}

bool Game::submitAnswer(LoggedUser user, int answerId, int timeToAnswer)
{
    bool ret = false;
    if (timeToAnswer == -1)
    {
        timeToAnswer = _data.timePerQuestion;
    }
    if (_players[user].correctAnswerCount + _players[user].wrongAnswerCount >= _data.numOfQuestions)
    {
        return false;
    }

    if (_players[user].currentQuetion.getAnswer(answerId) == _players[user].currentQuetion.getCorrectAnswer())
    {
        _players[user].correctAnswerCount++;
        ret = true;
    }
    else
    {
        _players[user].wrongAnswerCount++;
        ret = false;
    }
    if (_players[user].correctAnswerCount + _players[user].wrongAnswerCount < _data.numOfQuestions)
    {
        _players[user].currentQuetion = _questions[_players[user].correctAnswerCount + _players[user].wrongAnswerCount];
    }
    //equation for getting the new avrage given a new time
    _players[user].averegeAnswerTime = ((_players[user].wrongAnswerCount + _players[user].correctAnswerCount - 1) * _players[user].averegeAnswerTime + timeToAnswer) / (_players[user].wrongAnswerCount + _players[user].correctAnswerCount);
    for (auto player : _players)
    {
        LoggedUser u = player.first;
        std::cout << u.getUsername() << " Total ans: " << std::to_string(player.second.wrongAnswerCount + player.second.correctAnswerCount) << std::endl;
    }
    return ret;
}
void Game::RemovePlayer(LoggedUser user)
{
    if (_players.find(user) != _players.end())
    {
        _players.erase(user);
    }
}

Game::Game() //defult const
{
    _questions = std::vector<Question>();
    _gameId = 0;
    _players = std::map<LoggedUser, GameData>();
}

int Game::getId()
{
    return _gameId;
}

std::vector<PlayerResult> Game::getResults()
{
    std::vector<PlayerResult> results;
    for (auto pair : _players)
    {
        PlayerResult result;
        result.averageAnswerTime = pair.second.averegeAnswerTime;
        result.correctAnswerCount = pair.second.correctAnswerCount;
        result.wrongAnswerCount = pair.second.wrongAnswerCount;
        LoggedUser user = pair.first;
        result.username = user.getUsername();
        results.push_back(result);
    }
    return results;
}

bool Game::hasEnded()
{
    return _playersFinished == _players.size();
}