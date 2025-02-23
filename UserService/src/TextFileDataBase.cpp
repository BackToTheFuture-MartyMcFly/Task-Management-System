#include "TextFileDataBase.h"

TextFileDataBase::TextFileDataBase()
	:db(), dbPath(""), isConnected(false) {}

TextFileDataBase::~TextFileDataBase() {
	disconnect();
}

bool TextFileDataBase::connect(const std::string& connectionStr) {
	if (isConnected)
		return true;
	db.open(connectionStr, std::ios::in | std::ios::out | std::ios::app);
	if (db.is_open()) {
		isConnected = true;
		dbPath = connectionStr;
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

	if (isConnected) {
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
	if (!isConnected) {
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
	if (!isConnected) {
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
	if (!isConnected) {
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
	if (!isConnected) {
		std::cout << "log: [TextFileDataBase::deleteUserFromId()] DB not connected" << std::endl;
		return false;
	}

	std::filesystem::path dbPathFS(dbPath);
	std::filesystem::path tmpFilePath = dbPathFS.parent_path() / (dbPathFS.stem().string() + ".tmp");
	std::string tmpFileName = tmpFilePath.string();
	
	std::ofstream tmpFile(tmpFileName);
	if (!tmpFile.is_open()) {
		std::cout << "[TextFileDataBase::deleteUserFromId()] Could not open temporary file for writing: " << tmpFileName << std::endl;
		return false;
	}

	std::string userRecord;
	db.seekg(0);
	bool userFound = false;
	while (std::getline(db, userRecord)) {
		try {
			size_t sepPos = userRecord.find(",");
			if (sepPos == std::string::npos) {
				std::cout << "log: [TextFileDataBase::deleteUserFromId()] invalid record" << std::endl;
				tmpFile << userRecord << std::endl;
				continue;
			}
			int userId = stoi(userRecord.substr(0, sepPos));
			if (id == userId) {
				userFound = true;
				continue;
			}
			tmpFile << userRecord << std::endl;
		}
		catch (const std::invalid_argument& e) {
			std::cout << "log: [TextFileDataBase::deleteUserFromId()] Invalid argument: " << e.what() << " in record: " << userRecord << std::endl;
			tmpFile << userRecord << std::endl;
			continue;
		}
		catch (const std::out_of_range& e) {
			std::cout << "log: [TextFileDataBase::deleteUserFromId()] Out of range: " << e.what() << " in record: " << userRecord << std::endl;
			tmpFile << userRecord << std::endl;
			continue;
		}
		catch (const std::exception& e) {
			std::cout << "log: [TextFileDataBase::deleteUserFromId()] An unexpected exception occurred: " << e.what() << " .Skipping record: "  << userRecord << std::endl;
			tmpFile << userRecord << std::endl;
			continue;
		}
	}
	tmpFile.close();
	db.close();

	std::error_code errc;
	std::filesystem::remove(dbPath, errc);
	if (errc) {
		std::cout << "log: [TextFileDataBase::deleteUserFromId()] Could not delete original file: " << dbPath << " Error: " << errc.message() << std::endl;
		std::filesystem::remove(tmpFilePath, errc);
		db.open(dbPath);
		return false;
	}
	std::filesystem::rename(tmpFilePath, dbPath, errc);
	if (errc) {
		std::cout << "log: [TextFileDataBase::deleteUserFromId()] Could not rename temporary file: " << dbPath << " Error: " << errc.message() << std::endl;
		std::filesystem::remove(tmpFilePath, errc);
		db.open(dbPath);
		return false;
	}

	db.open(dbPath);

	std::cout << "log:[TextFileDataBase::deleteUserFromId()] User with id: " << id << " deleted successfully." << std::endl;
	return true;
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
		std::cout << "log: [TextFileDataBase::parseUserInfo()] Invalid argument: " << e.what() << " in record: " << userRecord << std::endl;
		return std::nullopt;
	}
	catch (const std::out_of_range& e) {
		std::cout << "log: [TextFileDataBase::parseUserInfo()] Out of range: " << e.what() << " in record: " << userRecord << std::endl;
		return std::nullopt;
	}
	catch (const std::exception& e) {
		std::cout << "log: [TextFileDataBase::parseUserInfo()] An unexpected exception occurred: " << e.what() << " in record: " << userRecord << std::endl;
		return std::nullopt;
	}
}


