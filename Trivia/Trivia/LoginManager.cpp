#include "LoginManager.h"

LoginManager::LoginManager(IDataBase* database) : _database(database)
{
}

bool LoginManager::signup(std::string username, std::string password, std::string email)
{
	try
	{
		if (!_database->doesUserExist(username))
		{
			_database->addNewUser(username, password, email);
			return true;
		}
	}
	catch(std::exception)
	{
		return false;
	}
	return false;
}

bool LoginManager::login(std::string username, std::string password)
{
	if (_database->doesPassWordExist(username,password))
	{
		for (auto user : _loggedUsers)
		{
			if (user.getUsername() == username)
			{
				return false;
			}
		}
		LoggedUser a = LoggedUser();
		a.setUserName(username);
		a.setPassWord(password);
		_loggedUsers.push_back(a);
		return true;
	}
	return false;
}

bool LoginManager::logout(std::string username)
{
	for (auto i = _loggedUsers.begin(); i != _loggedUsers.end(); i++)
	{
		if (i->getUsername() == username)
		{
			_loggedUsers.erase(i);
			return true;
		}
	}
	return false;
}



