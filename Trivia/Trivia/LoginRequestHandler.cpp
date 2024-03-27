#include "LoginRequestHandler.h"

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& factory) : _factory(factory)
{
}

bool LoginRequestHandler::isRequestRelevant(RequestInfo info)
{
	return info.id == LOGIN_REQUEST_CODE || info.id == SIGNUP_REQUEST_CODE || info.id == END_COMM;
}

IRequestHandler::RequestResult LoginRequestHandler::handleRequest(RequestInfo info)
{
	RequestResult req;
	ErrorResponse res;
	res.message = "Failed to login or signup.";
	req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
	req.newHandler = this;
	try
	{
		if (isRequestRelevant(info))
		{
			if (info.id == LOGIN_REQUEST_CODE)
			{
				req = login(info);
			}
			else if(info.id == SIGNUP_REQUEST_CODE)
			{
				req = signup(info);
			}
			else if (info.id == END_COMM)
			{
				req = signout(info);
			}
		}
	}
	catch (std::exception)
	{
	}
	return req;
}

IRequestHandler::RequestResult LoginRequestHandler::login(RequestInfo info)
{
	RequestResult req;
	LoginResponse res;
	LoggedUser user;
	LoginRequest userData = JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer);
	if (_factory.getLoginManager().login(userData.username, userData.password))
	{
		res.status = SUCCESS;
		user.setPassWord(userData.password);
		user.setUserName(userData.username);
		req.newHandler = _factory.createMenuRequestHandler(user);
	}
	else
	{
		res.status = FAIL;
		req.newHandler = this;
	}
	req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
	return req;
}

IRequestHandler::RequestResult LoginRequestHandler::signup(RequestInfo info)
{
	RequestResult req;
	SignupResponse res;
	LoggedUser user;
	SignupRequest userData = JsonRequestPacketDeserializer::deserializeSignupRequest(info.buffer);
	if (_factory.getLoginManager().signup(userData.username, userData.password, userData.email))
	{
		res.status = SUCCESS;
		user.setPassWord(userData.password);
		user.setUserName(userData.username);
		req.newHandler = _factory.createMenuRequestHandler(user);
	}
	else
	{
		res.status = FAIL;
		req.newHandler = this;
	}
	req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
	return req;
}

IRequestHandler::RequestResult LoginRequestHandler::signout(RequestInfo info)
{
	RequestResult req;
	LogoutResponse res;
	res.status = 1;
	req.buffer = JsonResponsePacketSerializer::serializeResponse(res);
	req.newHandler = nullptr;
	return req;
}
