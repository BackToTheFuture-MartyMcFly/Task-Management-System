#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <filesystem>

class Logger {
	private:
		static std::string m_service_name;
		static std::string m_log_dir;
		static std::shared_ptr<spdlog::logger> m_global_logger;
		static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> m_service_loggers;
		static bool m_initialize;

public:
	static void init(const std::string& service_name, 
					 std::string log_dir = "\\var\\log", 
					 spdlog::level::level_enum log_level = spdlog::level::info);

	static std::shared_ptr<spdlog::logger> getServiceLogger(const std::string& service_name = "");
	static std::shared_ptr<spdlog::logger> getGlobalLogger();
	static void createGlobalLogger();
	static std::shared_ptr<spdlog::logger> createServiceLogger(const std::string& service_name);
	static void setLevel(spdlog::level::level_enum level);
	static void shutdown();

};


#define LOG_INIT(...)           Logger::init(__VA_ARGS__)
#define LOG_SET_LEVEL(level)    Logger::setLevel(level)
#define LOG_SHUTDOWN()          Logger::shutdown()

#define LOG_INFO(...)       Logger::getServiceLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)       Logger::getServiceLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)      Logger::getServiceLogger()->error(__VA_ARGS__)
#define LOG_DEBUG(...)      Logger::getServiceLogger()->debug(__VA_ARGS__)
#define LOG_CRITICAL(...)   Logger::getServiceLogger()->critical(__VA_ARGS__)

#define GLOBAL_LOG_INFO(...)    Logger::getGlobalLogger()->info(__VA_ARGS__)
#define GLOBAL_LOG_WARN(...)    Logger::getGlobalLogger()->warn(__VA_ARGS__)
#define GLOBAL_LOG_ERROR(...)   Logger::getGlobalLogger()->error(__VA_ARGS__)