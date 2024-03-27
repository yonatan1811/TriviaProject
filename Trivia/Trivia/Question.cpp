#include "Question.h"

Question::Question(std::string question, std::string correct, std::string wrong1, std::string wrong2, std::string wrong3)
{
	_question = question;
	_possibleAnswers.push_back(correct);
	_possibleAnswers.push_back(wrong1);
	_possibleAnswers.push_back(wrong2);
	_possibleAnswers.push_back(wrong3);
	for (auto answer : _possibleAnswers)
	{
		unsigned int id = getRandomId();
		if (answer == _possibleAnswers[0])
			_correctId = id;
		std::pair<unsigned int, std::string> s(id, answer);
		_answersWithId.insert(s);
	}
}

int Question::getRandomId()
{
	srand(time(NULL));
	unsigned int id = 0;
	do
	{
		id = rand() % 4 + 1;
	} while (_answersWithId.find(id) != _answersWithId.end());
	return id;
}

std::string Question::getQuestion()
{
	return _question;
}

std::string Question::getPossibleAnswers()
{
	std::string answers = "";
	for (auto ans : _possibleAnswers)
	{
		answers += ans + ",";
	}
	//remove last ','
	if (answers.length() > 0)
	{
		answers.pop_back();
	}
	return answers;
}

std::string Question::getCorrectAnswer()
{
	return _possibleAnswers[0];
}

std::string Question::getAnswer(int id)
{
	if (_answersWithId.find(id) == _answersWithId.end())
		return "";
	return _answersWithId[id];
}

std::map<unsigned int, std::string> Question::getIdAnswers()
{
	return _answersWithId;
}
