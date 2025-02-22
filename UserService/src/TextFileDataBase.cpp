#include "TextDataBase.h"

TextFileDataBase::TextFileDataBase()
	:db(), fileName(""), isConnected(false) {}

TextFileDataBase::~TextFileDataBase() {
	disconnect();
}

bool TextFileDataBase::connect(const std::string& connectionStr) {
	if (isConnected)
		return true;
	db.open(connectionStr, std::ios::in | std::ios::out | std::ios::app);
	if (db.is_open()) {
		isConnected = true;
		fileName = connectionStr;
		return true;
	}
	else
		std::cout << "log: [TextFileDataBase::connect()] file not open" << std::endl;
		return false;
}

void TextFileDataBase::disconnect() {
	if (isConnected) {
		db.close();
		isConnected = false;
	}		
}

bool TextFileDataBase::isConnect() {
	return isConnected;
}

std::vector<User> TextFileDataBase::getUsersList() {
	std::vector<User> usersList;

	if (isConnect) {
		std::string userRecord;
		db.seekg(0);
		while (std::getline(db, userRecord)) {
			std::optional<User> user = parseUserInfo(userRecord);
			if (user.has_value())
				usersList.push_back(*user);
		}
		return usersList;
	}
	else {
		std::cout << "log: [TextFileDataBase::getUsersList()] DB not connected" << std::endl;
		return usersList;
	}
}

bool TextFileDataBase::isUnique(const User& user) {
	if (!isConnect) {
		std::cout << "log: [TextFileDataBase::isUnique()] DB not connected" << std::endl;
		return false;
	}

	std::string userRecord;
	db.seekg(0);
	while (std::getline(db, userRecord)) {
		std::optional<User> userTmp = parseUserInfo(userRecord);

		if (!userTmp.has_value()) {
			std::cout << "log: [TextFileDataBase::isUnique()] DB has invalid record" << std::endl;
			continue;
		}

		if (user.getId() == userTmp->getId() ||
			user.getName() == userTmp->getName() ||
			user.getEmail() == userTmp->getEmail())
		{
			return false;
		}
	}
	return true;
}


bool TextFileDataBase::createUser(const User& user) {
	if (!isConnect) {
		std::cout << "log: [TextFileDataBase::createUser()] DB not connected" << std::endl;
		return false;
	}
		
	if (!isUnique(user)) {
		std::cout << "log: [TextFileDataBase::createUser()] there is already such a user" << std::endl;			
		return false;
	}

	std::stringstream ss;
	ss << user.getId() << ","
		<< user.getName() << "," 
		<< user.getEmail() << ","
		<< user.getPasswordHash();

	db << ss.str() << std::endl;

	if (!db.good()) {
		std::cout << "Error: [TextFileDataBase::createUser()] Failed to write to database" << std::endl;
		return false;
	}
}

std::optional<User> TextFileDataBase::getUserFromId(int id) {
	std::optional<User> user;
	if (!isConnect) {
		std::cout << "log: [TextFileDataBase::getUserFromId()] DB not connected" << std::endl;
		return std::nullopt;
	}

	db.seekg(0);
	std::string userRecord;
	while (std::getline(db, userRecord)) {
		user = parseUserInfo(userRecord);
		
		if (!user.has_value()) {
			std::cout << "log: [TextFileDataBase::getUserFromId()] DB has invalid record" << std::endl;
			continue;
		}
	
		if (user->getId() == id)
			return *user;
	}

	std::cout << "log: [TextFileDataBase::getUserFromId()] there is no user with this id" << std::endl;
	return  std::nullopt;
}

bool TextFileDataBase::deleteUserFromId(int id) {
	std::vector<std::string> recordsInDB;
	std::string fileContent;
	db.seekg(0);
	while (std::getline(db, fileContent)) {
		if (atoi(fileContent.substr(0, fileContent.find(" ")).c_str()) != id)
			recordsInDB.push_back(fileContent);
	}
	disconnect();
	std::string tmpFileName = fileName.substr(0, fileName.find(".") + 1) + "tmp";
	std::ofstream db_tmp(tmpFileName);
	if (db_tmp.is_open()) {
		for (auto i : recordsInDB)
			db_tmp << i << std::endl;
		db_tmp.close();
	}
	else {
		std::cout << "No deleting user: file tmp no open" << std::endl;
		return;
	}

	if (std::remove(fileName.c_str()) != 0) {
		std::cerr << "Error: Could not delete old file." << std::endl;
		std::remove(tmpFileName.c_str());
		return;
	}

	if (std::rename(tmpFileName.c_str(), fileName.c_str()) != 0) { //опасно
		std::cout << "Error: Could not rename new file" << std::endl;
		return;
	}

	connect(fileName); // для единообразия, после операции удаления мы снова подключены к базе
}

bool TextFileDataBase::updateUser(const User& user) {
	std::vector<std::string> recordsInDB;
	std::string fileContent;
	while (std::getline(db, fileContent)) {
		//if (atoi(fileContent.substr(0, fileContent.find(" ")).c_str()) != id)
			recordsInDB.push_back(fileContent);
	}
}

std::optional<User> TextFileDataBase::parseUserInfo(const std::string& userRecord) {
	std::vector<std::string> userInfo;
	std::stringstream ss(userRecord);
	std::string token;
	while (std::getline(ss, token, ','))
		userInfo.push_back(token);

	if (userInfo.size() != 4) {
		std::cout << "log: [TextFileDataBase::parseUserInfo()] Record of User in DB no full" << std::endl;
		return std::nullopt;
	}
	try {
		int UserId = stoi(userInfo[0]);
		std::string UserName = userInfo[1];
		std::string UserEmail = userInfo[2];

		User result(UserId, UserName, UserEmail);
		result.setPasswordHash(userInfo[3]);

		return result;
	}
	catch (const std::invalid_argument& e) {
		std::cout << "Error: Invalid argument: " << e.what() << " in record: " << userRecord << std::endl;
		return std::nullopt;
	}
	catch (const std::out_of_range& e) {
		std::cout << "Error: Out of range: " << e.what() << " in record: " << userRecord << std::endl;
		return std::nullopt;
	}
}


