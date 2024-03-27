#pragma once
#include "RoomStructs.h"
#include <vector>
#include "LoggedUser.h"
#include <string>

class Room
{
public:
	Room();
	Room(RoomData data);
	void addUser(LoggedUser);
	void removeUser(LoggedUser);
	std::vector<std::string> getAllUsers();
	RoomData& getData();
	std::vector<LoggedUser> getLoggedUsers();
	bool operator < (const Room& r);

private:
	RoomData m_metadata;
	std::vector<LoggedUser> m_users;


};
