#pragma once

#include <iostream>

class LoggedUser
{
private:
	std::string _username;
	std::string password;
public:
	std::string getUsername();
	std::string getPassWord();
	void setUserName(std::string user);
	void setPassWord(std::string pass);
	bool operator < (const LoggedUser& lu) const;

};

