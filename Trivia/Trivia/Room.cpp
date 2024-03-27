#include "Room.h"

Room::Room()
{

}

Room::Room(RoomData data)
{
	m_metadata = data;
}

void Room::addUser(LoggedUser Lu)
{
	m_users.push_back(Lu);
}

RoomData& Room::getData()
{
	return m_metadata;
}

void Room::removeUser(LoggedUser Lu)
{
	auto it = m_users.begin();
	for (auto i:m_users)
	{
		if (i.getUsername() == Lu.getUsername() && i.getPassWord() == Lu.getPassWord())
		{
			m_users.erase(it);
			return;
		}
		it++;
	}
}

std::vector<std::string> Room::getAllUsers()
{
	std::vector<std::string> vecOfUsers;
	for (auto i : m_users )
	{
		vecOfUsers.push_back(i.getUsername());
	}
	return vecOfUsers;
}

std::vector<LoggedUser> Room::getLoggedUsers()
{
	return m_users;
}

bool Room::operator<(const Room& r)
{
	return this->getData().id < r.m_metadata.id;
}
