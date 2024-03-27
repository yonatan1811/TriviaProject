#pragma once
#include <map>
#include <cstdlib>
#include <iostream>
#include "Room.h"


class RoomManager
{
public:

	int createRoom(LoggedUser LU, RoomData Rd);
	void deleteRoom(int ID);
	unsigned int getRoomState(int id);
	std::vector<RoomData> getRooms();
	Room& getRoom(int ID);

private:
	int getRandomRoomId();
	std::map <int, Room> m_rooms;

};
