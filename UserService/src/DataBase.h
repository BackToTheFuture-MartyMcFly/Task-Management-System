#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <optional>

#include "User.h"
#include "Logger.h"

class DataBase {

public:
	virtual ~DataBase() = default;
	virtual bool connect(const std::string& ) = 0;
	virtual void disconnect() = 0;
	virtual bool isConnect() = 0;
	virtual std::vector<User> getUsersList() = 0;
	virtual bool isUnique(const User&) = 0;
	virtual bool createUser(const User&) = 0;
	virtual std::optional<User> getUserFromId(int) = 0;
	virtual bool deleteUserFromId(int) = 0;
	virtual bool updateUserFromId(int, const User&) = 0;
	

protected:
	DataBase() = default;

};