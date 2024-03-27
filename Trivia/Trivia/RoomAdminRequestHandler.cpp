#include "RoomAdminRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(Room* room, LoggedUser& user, RequestHandlerFactory& factory) : _factory(factory)
{
	_room = room;
	_user = user;
}

bool RoomAdminRequestHandler::isRequestRelevant(IRequestHandler::RequestInfo info)
{
	return info.id == CLOSE_ROOM_REQUEST_CODE || info.id == START_GAME_REQUEST_CODE || info.id == GET_ROOM_STATE_REQUEST_CODE || info.id == END_COMM;
}

IRequestHandler::RequestResult RoomAdminRequestHandler::handleRequest(IRequestHandler::RequestInfo info)
{
	RequestResult req;
	req.buffer = std::vector<unsigned char>();
	req.newHandler = nullptr;
	try
	{
		if (isRequestRelevant(info))
		{
			if (info.id == CLOSE_ROOM_REQUEST_CODE)
			{
				req = closeRoom(info);
			}
			else if (info.id == START_GAME_REQUEST_CODE)
			{
				req = startGame(info);
			}
			else if (info.id == GET_ROOM_STATE_REQUEST_CODE)
			{
				req = getRoomState(info);
			}
			else if (info.id == END_COMM)
			{
				req = logOut(info);
			}
		}
		else
		{
			ErrorResponse res;
			res.message = "Failed to use room admin option.";
			req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
			req.newHandler = this;
		}
	}
	catch (std::exception)
	{
		ErrorResponse res;
		res.message = "Failed to use room admin option.";
		req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
		req.newHandler = this;
	}
	return req;
}

IRequestHandler::RequestResult RoomAdminRequestHandler::closeRoom(IRequestHandler::RequestInfo info)
{
	_room->getData().isActive = OFFLINE;
    _room->removeUser(_user);
	CloseRoomResponse res;
	res.status = SUCCESS;
	RequestResult req;
	req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
	req.newHandler = _factory.createMenuRequestHandler(_user);
	return req;
}

IRequestHandler::RequestResult RoomAdminRequestHandler::startGame(IRequestHandler::RequestInfo info)
{
	_room->getData().isActive = RUNNING_GAME;
	StartGameResponse res;
	RequestResult req;
	res.status = SUCCESS;
	req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
	req.newHandler = _factory.createGameRequestHandler(*_room,_user);
	return req;
}

IRequestHandler::RequestResult RoomAdminRequestHandler::getRoomState(IRequestHandler::RequestInfo info)
{
	RoomData data = _room->getData();
	GetRoomStateResponse res;
	RequestResult req;
	res.answerTimeout = data.timePerQuestion;
	res.hasGameBegun = data.isActive;
	res.players = _room->getAllUsers();
	res.questionCount = data.numOfQuestions;
	res.status = SUCCESS;
	req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
	req.newHandler = this;
	return req;
}

IRequestHandler::RequestResult RoomAdminRequestHandler::logOut(RequestInfo info)
{
	RequestResult req;
	LogoutResponse res;
	res.status = 1;
	req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
	req.newHandler = nullptr;
	closeRoom(info);
	_factory.getLoginManager().logout(_user.getUsername());
	return req;
}