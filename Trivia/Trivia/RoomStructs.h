#pragma once

#include <string>

#define OFFLINE 0
#define WAITING_TO_START 1
#define RUNNING_GAME 2

struct RoomData
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestions;
	unsigned int timePerQuestion;
	unsigned int isActive;
};

