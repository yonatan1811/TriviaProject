#include "RoomManager.h"

int RoomManager::getRandomRoomId()
{
    srand(time(NULL));
    unsigned int id = 0;
    do
    {
        id = rand();
    } while (m_rooms.find(id) != m_rooms.end());
    return id;
}

int RoomManager::createRoom(LoggedUser LU, RoomData Rd)
{
    Rd.id = getRandomRoomId();
    Room room(Rd);
    room.addUser(LU);
    m_rooms.insert(std::pair<int, Room>(Rd.id, room));
    return Rd.id;
}
void RoomManager::deleteRoom(int ID)
{
    for (auto it = m_rooms.begin(); it != m_rooms.end(); it++)
    {
        if (it->first == ID)
        {
            m_rooms.erase(it);
            return;
        }
    }
}
unsigned int RoomManager::getRoomState(int id)
{
    return m_rooms[id].getData().id;
}

std::vector<RoomData> RoomManager::getRooms()
{
    std::vector<RoomData> rooms;
    std::vector<int> toDelete;
    for (std::pair<int, Room> pair : m_rooms)
    {
        Room curr = pair.second;
        if(curr.getLoggedUsers().size() > 0 && curr.getData().isActive == WAITING_TO_START) 
        {
            rooms.push_back(pair.second.getData());
        }
        else
        {
            toDelete.push_back(pair.first);
        }
    }
    for (auto id : toDelete)
    {
        deleteRoom(id);
    }
    return rooms;
}
Room& RoomManager::getRoom(int ID)
{
    return m_rooms[ID];
}