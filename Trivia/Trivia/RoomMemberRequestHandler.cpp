#include "RoomMemberRequestHandler.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(Room* room, LoggedUser& user, RequestHandlerFactory& handle) : _room(room),_user(user),_factory(handle)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(RequestInfo info)
{
    return info.id == GET_ROOM_STATE_REQUEST_CODE || info.id == LEAVE_ROOM_REQUEST_CODE || info.id == END_COMM;
}

IRequestHandler::RequestResult RoomMemberRequestHandler::handleRequest(RequestInfo info)
{
	RequestResult req;
	req.buffer = std::vector<unsigned char>();
	req.newHandler = nullptr;
	try
	{
		if (isRequestRelevant(info))
		{
			if (info.id == LEAVE_ROOM_REQUEST_CODE)
			{
				req = leaveRoom(info);
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
			res.message = "Failed to use room member option.";
			req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
			req.newHandler = this;
		}
	}
	catch (std::exception)
	{
		ErrorResponse res;
		res.message = "Failed to use room member option.";
		req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
		req.newHandler = this;
	}
	return req;
}

IRequestHandler::RequestResult RoomMemberRequestHandler::leaveRoom(RequestInfo info)
{
	_room->removeUser(_user);
	LeaveRoomResponse res;
	RequestResult req;
	res.status = SUCCESS;
	req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
	req.newHandler = _factory.createMenuRequestHandler(_user);
	return req;
}

IRequestHandler::RequestResult RoomMemberRequestHandler::getRoomState(RequestInfo info)
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
	if (data.isActive == OFFLINE)
	{
		leaveRoom(info);
		req.newHandler = _factory.createMenuRequestHandler(_user);
	}
	else if (data.isActive == RUNNING_GAME)
	{
		req.newHandler = _factory.createGameRequestHandler(_user);
	}
	return req;
}

IRequestHandler::RequestResult RoomMemberRequestHandler::logOut(RequestInfo info)
{
	RequestResult req;
	LogoutResponse res;
	res.status = 1;
	req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
	req.newHandler = nullptr;
	leaveRoom(info);
	_factory.getLoginManager().logout(_user.getUsername());
	return req;
}
