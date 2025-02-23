#pragma once

#include <string>
#include "DataBase.h"
#include <fstream>
#include <filesystem>
#include <sstream>

class TextFileDataBase : public DataBase {

public:
	TextFileDataBase();

	~TextFileDataBase() override;
	virtual bool connect(const std::string&) override;
	virtual void disconnect() override;
	virtual bool isConnect() override;
	virtual std::vector<User> getUsersList() override;
	virtual bool isUnique(const User&) override;
	virtual bool createUser(const User&) override;
	virtual std::optional<User> getUserFromId(int) override;
	virtual bool deleteUserFromId(int) override;
	virtual bool updateUserFromId(int, const User&) override;
	
	std::optional<User> parseUserInfo(const std::string&);

private:
	std::fstream db;
	std::string dbPath;
	bool connection;


};