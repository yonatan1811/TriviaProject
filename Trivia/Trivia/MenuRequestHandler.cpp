#include "MenuRequestHandler.h"

MenuRequestHandler::MenuRequestHandler(LoggedUser& user, RequestHandlerFactory& factory) : _factory(factory), _user(user)
{
}

bool MenuRequestHandler::isRequestRelevant(IRequestHandler::RequestInfo info)
{
    return info.id == CREATE_ROOM_REQUEST_CODE || info.id == GET_ROOMS_REQUEST_CODE || info.id == GET_PLAYERS_REQUEST_CODE ||
        info.id == JOIN_ROOM_REQUEST_CODE || info.id == GET_STATS_REQUEST_CODE || info.id == LOGOUT_REQUEST_CODE || info.id == GET_HIGH_SCORES_REQUEST_CODE || info.id == END_COMM;
}
IRequestHandler::RequestResult MenuRequestHandler::handleRequest(IRequestHandler::RequestInfo info)
{
    RequestResult req;
    req.buffer = std::vector<unsigned char>();
    req.newHandler = nullptr;
    try
    {
        if (isRequestRelevant(info))
        {
            if (info.id == CREATE_ROOM_REQUEST_CODE)
            {
                req = createRoom(info);
            }
            else if (info.id == GET_ROOMS_REQUEST_CODE)
            {
                req = getRooms(info);
            }
            else if (info.id == GET_PLAYERS_REQUEST_CODE)
            {
                req = getPlayersInRoom(info);
            }
            else if (info.id == JOIN_ROOM_REQUEST_CODE)
            {
                req = joinRoom(info);
            }
            else if (info.id == GET_STATS_REQUEST_CODE)
            {
                req = getPersonalStats(info);
            }
            else if (info.id == LOGOUT_REQUEST_CODE)
            {
                req = signout(info);
            }
            else if (info.id == GET_HIGH_SCORES_REQUEST_CODE)
            {
                req = getHighScore(info);
            }
            else if (info.id == END_COMM)
            {
                req = logOut(info);
            }
        }
        else
        {
            ErrorResponse res;
            res.message = "Failed to use menu option.";
            req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
            req.newHandler = this;
        }
    }
    catch (std::exception)
    {
        ErrorResponse res;
        res.message = "Failed to use menu option.";
        req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
        req.newHandler = this;
    }
    return req;
}

IRequestHandler::RequestResult MenuRequestHandler::signout(RequestInfo info)
{
    RequestResult req;
    LogoutResponse res;
    _factory.getLoginManager().logout(_user.getUsername());
    res.status = 1;
    req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
    req.newHandler = this;
    return req;
}

IRequestHandler::RequestResult MenuRequestHandler::getRooms(RequestInfo info)
{
    RequestResult req;
    GetRoomsResponse res;
    res.rooms = _factory.getRoomManager().getRooms();
    res.status = 1;
    req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
    req.newHandler = this;
    return req;
}

IRequestHandler::RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo info)
{
    RequestResult req;
    GetPlayersInRoomRequest data = JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(info.buffer);
    GetPlayersInRoomResponse res;
    res.players = _factory.getRoomManager().getRoom(data.roomID).getAllUsers();
    req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
    req.newHandler = this;
    return req;
}

IRequestHandler::RequestResult MenuRequestHandler::getPersonalStats(RequestInfo info)
{
    RequestResult req;
    getPersonalStatsResponse res;
    res.statistics = _factory.getStatisticManager().getUserStatistics(_user.getUsername());
    res.status = 1;
    req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
    req.newHandler = this;
    return req;
}

IRequestHandler::RequestResult MenuRequestHandler::getHighScore(RequestInfo info)
{
    RequestResult req;
    getHighScoreResponse res;
    res.statistics = _factory.getStatisticManager().getHighScore();
    res.status = 1;
    req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
    req.newHandler = this;
    return req;
}

IRequestHandler::RequestResult MenuRequestHandler::joinRoom(RequestInfo info)
{
    RequestResult req;
    JoinRoomRequest data = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(info.buffer);
    JoinRoomResponse res;
    if (_factory.getRoomManager().getRoom(data.roomID).getLoggedUsers().size() < _factory.getRoomManager().getRoom(data.roomID).getData().maxPlayers)
    {
        _factory.getRoomManager().getRoom(data.roomID).addUser(_user);
        res.status = 1;
        req.newHandler = _factory.createRoomMemberRequestHandler(&(_factory.getRoomManager().getRoom(data.roomID)), _user);
    }
    else
    {
        res.status = 0;
        req.newHandler = this;
    }
    req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
    return req;
}

IRequestHandler::RequestResult MenuRequestHandler::createRoom(RequestInfo info)
{
    RequestResult req;
    CreateRoomRequest roomD = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(info.buffer);
    RoomData roomData;
    roomData.isActive = WAITING_TO_START;
    roomData.maxPlayers = roomD.maxUsers;
    roomData.name = roomD.roomName;
    roomData.numOfQuestions = roomD.questionCount;
    roomData.timePerQuestion = roomD.answerTimeOut;
    int id = _factory.getRoomManager().createRoom(_user, roomData);
    CreateRoomResponse res;
    res.status = 1;
    req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
    req.newHandler = _factory.createRoomAdminRequestHandler(&(_factory.getRoomManager().getRoom(id)), _user);
    return req;
}

IRequestHandler::RequestResult MenuRequestHandler::logOut(RequestInfo info)
{
    RequestResult req;
    LogoutResponse res;
    res.status = 1;
    req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
    req.newHandler = nullptr;
    _factory.getLoginManager().logout(_user.getUsername());
    return req;
}