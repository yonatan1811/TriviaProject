#include "LoggedUser.h"


std::string LoggedUser::getUsername()
{
	return _username;
}


void LoggedUser::setUserName(std::string user)
{
	_username = user;
}

std::string LoggedUser::getPassWord()
{
	return password;
}


void LoggedUser::setPassWord(std::string pass)
{
	password = pass;
}

bool LoggedUser::operator<(const LoggedUser& lu) const
{
	return _username < lu._username;
}


