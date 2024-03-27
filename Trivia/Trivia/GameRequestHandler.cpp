#include "GameRequestHandler.h"

GameRequestHandler::GameRequestHandler(LoggedUser& user, RequestHandlerFactory& rhf) : _factory(rhf) , _user(user) , _game(rhf.getGameManager().getGameForUser(user))
{
}

GameRequestHandler::GameRequestHandler(Room& room, LoggedUser& user, RequestHandlerFactory& rhf) : _factory(rhf), _user(user), _game(rhf.getGameManager().createGame(room))
{
}

bool GameRequestHandler::isRequestRelevant(RequestInfo info)
{
	if (info.id == LEAVE_GAME_CODE || info.id == GET_QUETION_RESPONSE_CODE || info.id == SUBMIT_ANSWER_RESPONSE || info.id == GET_GAME_RESULTS_CODE || info.id == HAS_GAME_ENDED_RESPONSE_CODE)
	{
		return true;
	}
	return false;
}

IRequestHandler::RequestResult GameRequestHandler::handleRequest(RequestInfo info)
{
	RequestResult req;
	req.buffer = std::vector<unsigned char>();
	req.newHandler = nullptr;
	try
	{
		if (isRequestRelevant(info))
		{
			if (info.id == LEAVE_GAME_CODE)
			{
				req = leaveGame(info);
			}
			else if (info.id == GET_QUETION_RESPONSE_CODE)
			{
				req = getQuestion(info);
			}
			else if (info.id == SUBMIT_ANSWER_RESPONSE)
			{
				req = submitAnswer(info);
			}
			else if (info.id == GET_GAME_RESULTS_CODE)
			{
				req = getGameResults(info);
			}
			else if (info.id == HAS_GAME_ENDED_RESPONSE_CODE)
			{
				req = hasEnded(info);
			}
		}
		else
		{
			ErrorResponse res;
			res.message = "Failed to use game option.";
			req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
			req.newHandler = this;
		}
	}
	catch (std::exception)
	{
		ErrorResponse res;
		res.message = "Failed to use game option.";
		req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
		req.newHandler = this;
	}
	return req;
}

IRequestHandler::RequestResult GameRequestHandler::getQuestion(RequestInfo info)
{
	RequestResult req;
	Question q = _game.getQuestionForUser(_user);
	GetQuestionResponse res;

	res.answers = q.getIdAnswers();
	res.question = q.getQuestion();
	if (q.getQuestion() == "ERROR")
	{
		res.status = FAIL;
	}
	else
	{
		res.status = SUCCESS;
	}
	req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
	req.newHandler = this;
	return req;
}
IRequestHandler::RequestResult GameRequestHandler::submitAnswer(RequestInfo info)
{
	RequestResult req;
	SubmitAnswerRequest request = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(info.buffer);
	SubmitAnswerResponse res;
	if (_game.submitAnswer(_user, request.answerId, request.timeToAnswer))
	{
		res.status = SUCCESS;
	}
	else
	{
		res.status = FAIL;
	}
	res.correctAnswerId = 0;
	req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
	req.newHandler = this;
	return req;
}

IRequestHandler::RequestResult GameRequestHandler::getGameResults(RequestInfo info)
{
	RequestResult req;
	GetGameResultsResponse res;
	res.results = _game.getResults();
	res.status = 1;
	req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
	req.newHandler = this;
	return req;
}

IRequestHandler::RequestResult GameRequestHandler::leaveGame(RequestInfo info)
{
	RequestResult req;
	req.newHandler = _factory.createMenuRequestHandler(_user, _game.getUserData(_user));
	_game.RemovePlayer(_user);
	LeaveGameResponse res;
	res.status = 1;
	req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
	return req;
}

IRequestHandler::RequestResult GameRequestHandler::hasEnded(RequestInfo info)
{
	RequestResult req;
	HasGameEndedResponse res;
	while(!_game.hasEnded())
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	res.hasEnded = SUCCESS;
	req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
	req.newHandler = this;
	return req;
}

IRequestHandler::RequestResult GameRequestHandler::logOut(RequestInfo info)
{
	RequestResult req;
	LogoutResponse res;
	res.status = 1;
	req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
	req.newHandler = nullptr;
	leaveGame(info);
	_factory.getLoginManager().logout(_user.getUsername());
	return req;
}
