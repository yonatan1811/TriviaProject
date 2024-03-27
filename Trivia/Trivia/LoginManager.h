#pragma once

#include <iostream>
#include <vector>
#include "LoggedUser.h"
#include "IDataBase.h"
#include "DataBaseAccess.h"

class LoginManager
{
public:
	LoginManager(IDataBase* database);
	bool signup(std::string username, std::string password, std::string email);
	bool login(std::string username, std::string password);
	bool logout(std::string username);

private:
	
	IDataBase* _database;
	std::vector<LoggedUser> _loggedUsers;
};