#pragma once

#include <iostream>
#include <vector>
#include <map>

class Question
{
private:
	std::string _question;
	std::vector<std::string> _possibleAnswers;
	std::map<unsigned int, std::string> _answersWithId;
	int _correctId;
	int getRandomId();
public:
	Question(std::string, std::string, std::string, std::string, std::string);
	std::string getQuestion();
	std::string getPossibleAnswers();
	std::string getCorrectAnswer();
	std::string getAnswer(int id);
	std::map<unsigned int, std::string> getIdAnswers();
};
