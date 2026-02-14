#include "TextFileDataBase.h"

TextFileDataBase::TextFileDataBase()
	:db(), dbPath(""), connection(false) {}

TextFileDataBase::~TextFileDataBase() {
	disconnect();
}

bool TextFileDataBase::connect(const std::string& connectionStr) {
	if (isConnect()) {
		LOG_INFO("The connection has already been established");;
		return true;
	}

	std::filesystem::path filePath(dbPath);

	if (std::filesystem::exists(filePath) && std::filesystem::file_size(filePath) == 0) {
		LOG_INFO("Database file is empty.");
	}
		
	db.open(connectionStr, std::ios::in | std::ios::out | std::ios::app);
	if (db.is_open()) {
		connection = true;
		dbPath = connectionStr;
		LOG_INFO("The connection is established");
		return true;
	}
	else
		LOG_INFO("Connection is not established");
		return false;
}

void TextFileDataBase::disconnect() {
	if (isConnect()) {
		db.close(); 
		connection = false;
		LOG_INFO("The connection is broken");
	}		
}

bool TextFileDataBase::isConnect() {
	return connection;
}

std::vector<User> TextFileDataBase::getUsersList() {
	std::vector<User> usersList;

	if (isConnect()) {
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
		LOG_WARN("DB not connected");
		return usersList;
	}
}

bool TextFileDataBase::isUnique(const User& user) {
	if (!isConnect()) {
		LOG_WARN("DB not connected");
		return false;
	}

	std::string userRecord;
	db.seekg(0);
	while (std::getline(db, userRecord)) {
		std::optional<User> userTmp = parseUserInfo(userRecord);

		if (!userTmp.has_value()) {
			LOG_WARN("DB has invalid record");
			continue;
		}

		if (user.getId() == userTmp->getId() ||
			user.getName() == userTmp->getName() ||
			user.getEmail() == userTmp->getEmail())
		{
			disconnect();
			connect(dbPath);
			return false;
		}
	}
	disconnect();
	connect(dbPath);
	return true;
}


bool TextFileDataBase::createUser(const User& user) {
	if (!isConnect()) {
		LOG_WARN("DB not connected");
		return false;
	}

	if (!isUnique(user)) {
		LOG_WARN("There is already such a user");
		return false;
	}

	std::stringstream ss;
	ss << user.getId() << ","
		<< user.getName() << "," 
		<< user.getEmail() << ","
		<< user.getPasswordHash();

	db << ss.str() << '\n';

	if (!db.good()) {
		LOG_WARN("Failed to write to database");
		return false;
	}
}

std::optional<User> TextFileDataBase::getUserFromId(int id) {
	std::optional<User> user;
	if (!isConnect()) {
		LOG_WARN("DB not connected");
		return std::nullopt;
	}

	db.seekg(0);
	std::string userRecord;
	while (std::getline(db, userRecord)) {
		user = parseUserInfo(userRecord);
		
		if (!user.has_value()) {
			LOG_WARN("DB has invalid record");
			continue;
		}
	
		if (user->getId() == id)
			return *user;
	}

	LOG_INFO("There is no user with this id");
	return  std::nullopt;
}

bool TextFileDataBase::deleteUserFromId(int id) {
	if (!isConnect()) {
		LOG_WARN("DB not connected");
		return false;
	}

	std::filesystem::path dbPathFS(dbPath);
	std::filesystem::path tmpFilePath = dbPathFS.parent_path() / (dbPathFS.stem().string() + ".tmp");
	std::string tmpFileName = tmpFilePath.string();
	
	std::ofstream tmpFile(tmpFileName);
	if (!tmpFile.is_open()) {
		LOG_WARN("Could not open temporary file for writing: {}", tmpFileName);
		return false;
	}

	std::string userRecord;
	db.seekg(0);
	bool userFound = false;
	while (std::getline(db, userRecord)) {
		std::optional<User> userTmp = parseUserInfo(userRecord);
			
		if (!userTmp.has_value()) {
			LOG_WARN("DB has invalid record");
			continue;
		}

		if (userTmp->getId() == id) {
			userFound = true;
			continue;
		}
		tmpFile << userRecord << std::endl;
			
	}

	tmpFile.close();
	db.close();

	std::error_code errc;
	std::filesystem::remove(dbPath, errc);
	if (errc) {
		LOG_ERROR("Could not delete original file: {} Error: {}", dbPath, errc.message());
		std::filesystem::remove(tmpFilePath, errc);
		db.open(dbPath);
		return false;
	}
	std::filesystem::rename(tmpFilePath, dbPath, errc);
	if (errc) {
		LOG_ERROR("Could not rename temporary file: {} Error: {}", dbPath, errc.message());
		std::filesystem::remove(tmpFilePath, errc);
		db.open(dbPath);
		return false;
	}

	db.open(dbPath);

	LOG_INFO("User with id: {} deleted successfully.", id);
	return true;
}

bool TextFileDataBase::updateUserFromId(int id, const User& user) {
	if (!isConnect()) {
		LOG_INFO("DB not connected");
		return false;
	}

	std::filesystem::path dbPathFS(dbPath);
	std::filesystem::path tmpFilePath = dbPathFS.parent_path() / (dbPathFS.stem().string() + ".tmp");
	std::string tmpFileName = tmpFilePath.string();

	std::ofstream tmpFile(tmpFileName);
	if (!tmpFile.is_open()) {
		LOG_WARN("Could not open temporary file for writing: {}",  tmpFileName);
		return false;
	}

	std::string userRecord;
	db.seekg(0);
	bool userUpdate = false;
	while (std::getline(db, userRecord)) {
		std::optional<User> userTmp = parseUserInfo(userRecord);
	
		if (!userTmp.has_value()) {
			LOG_WARN("DB has invalid record: {}", userRecord);
			tmpFile << userRecord << std::endl;
			continue;
		}

		if (userTmp->getId() == id) {
			tmpFile << user.getId() << "," << user.getName() << "," << user.getEmail() << "," << user.getPasswordHash() << std::endl;
			userUpdate = true;
			continue;
		}
			
		tmpFile << userRecord << std::endl;

	}

	tmpFile.close();
	db.close();

	std::error_code errc;
	std::filesystem::remove(dbPath, errc);
	if (errc) {
		LOG_ERROR("Could not delete original file: {} Error: {}", dbPath, errc.message());
		std::filesystem::remove(tmpFilePath, errc);
		db.open(dbPath);
		return false;
	}
	std::filesystem::rename(tmpFilePath, dbPath, errc);
	if (errc) {
		LOG_ERROR("Could not rename temporary file: {} Error: {}", dbPath, errc.message());
		std::filesystem::remove(tmpFilePath, errc);
		db.open(dbPath);
		return false;
	}

	db.open(dbPath);

	if(!userUpdate)
		LOG_WARN("Don`t have user with id: {}", id);

	return userUpdate;
}

std::optional<User> TextFileDataBase::parseUserInfo(const std::string& userRecord) {
	std::vector<std::string> userInfo;
	std::stringstream ss(userRecord);
	std::string token;
	while (std::getline(ss, token, ','))
		userInfo.push_back(token);

	if (userInfo.size() != 4) {
		LOG_ERROR("Record of User in DB no full");
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
		LOG_ERROR("Invalid argument: {} in record: {}", e.what(), userRecord);
		return std::nullopt;
	}
	catch (const std::out_of_range& e) {
		LOG_ERROR("Out of range: {} in record: {}", e.what(), userRecord);
		return std::nullopt;
	}
	catch (const std::exception& e) {
		LOG_ERROR("An unexpected exception occurred: {} in record: {}", e.what(), userRecord);
		return std::nullopt;
	}
}


