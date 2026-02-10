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
					 const std::string& log_dir = "./var/log", 
					 spdlog::level::level_enum log_level = spdlog::level::info);

	static std::shared_ptr<spdlog::logger> getServiceLogger(const std::string& service_name = "");
	static std::shared_ptr<spdlog::logger> getGlobalLogger();
	static void createGlobalLogger();
	static std::shared_ptr<spdlog::logger> createServiceLogger(const std::string& service_name);
	static void setLevel(spdlog::level::level_enum level);
	static void shutdown();



};