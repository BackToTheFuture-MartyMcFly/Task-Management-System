#include "Logger.h"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class LoggerTest : public testing::Test
{
protected:
	void SetUp() override {
		name_test_service = "test_service_" + std::to_string(::getpid());
		log_path = fs::path("../log");
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
	Logger::init(name_test_service);
	EXPECT_TRUE(fs::exists(log_path));
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}