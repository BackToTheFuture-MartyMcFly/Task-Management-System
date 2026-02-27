#include "Logger.h"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

#define XSTR(s) STR(s)
#define STR(s) #s
#define NAME XSTR(SERVICE_NAME)

class LoggerTest : public testing::Test
{
protected:
	void SetUp() override {
		name_test_service = NAME;
		log_path = fs::path("../logs");
		if (fs::exists(log_path))
			fs::remove_all(log_path);
	}

	void TearDown() override {
		Logger::shutdown();
		if (fs::exists(log_path))
			fs::remove_all(log_path);

	}

	std::string readLogFile() {
		auto fileName = (log_path / (name_test_service + ".log")).string();
		std::ifstream file(fileName);
		if (!file.is_open())
			return std::string();
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	std::string name_test_service;
    fs::path log_path;
};

TEST_F(LoggerTest, CreateLogFiles) {
	EXPECT_FALSE(fs::exists(log_path));
	LOG_INIT(name_test_service);
	EXPECT_TRUE(fs::exists(log_path));
}

TEST_F(LoggerTest, CheckingLogContentError) {
	LOG_INIT(name_test_service);
	auto LogMessage = "Test log message error";
	LOG_ERROR("{}", LogMessage);
	std::string LogContent = readLogFile();
	EXPECT_TRUE(LogContent.contains(LogMessage));
}

TEST_F(LoggerTest, CheckingLogContentInfo) {
	LOG_INIT(name_test_service);
	auto LogMessage = "Test log message info";
	LOG_INFO("{}", LogMessage);
	std::string LogContent = readLogFile();
	EXPECT_TRUE(LogContent.contains(LogMessage));
}

TEST_F(LoggerTest, CheckingLogContentWarn) {
	LOG_INIT(name_test_service);
	auto LogMessage = "Test log message warn";
	LOG_WARN("{}", LogMessage);
	std::string LogContent = readLogFile();
	EXPECT_TRUE(LogContent.contains(LogMessage));
}


int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}