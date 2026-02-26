#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <filesystem>

namespace fs = std::filesystem;

class Logger {
	private:
		static std::shared_ptr<spdlog::logger> m_global_logger;
		static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> m_service_loggers;
		static bool m_initialize_global_log;

public:
	static void init(const std::string& service_name, spdlog::level::level_enum log_level = spdlog::level::info);
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

#define SRV_LOG_INFO(...)		Logger::getServiceLogger()->info("[{}:{}] {}", \
    fs::path(__FILE__).filename().string(), \
    __LINE__, \
    fmt::format(__VA_ARGS__))
#define SRV_LOG_WARN(...)		Logger::getServiceLogger()->warn("[{}:{}] {}", \
    fs::path(__FILE__).filename().string(), \
    __LINE__, \
    fmt::format(__VA_ARGS__))
#define SRV_LOG_DEBUG(...)		Logger::getServiceLogger()->debug("[{}:{}] {}", \
    fs::path(__FILE__).filename().string(), \
    __LINE__, \
    fmt::format(__VA_ARGS__))
#define SRV_LOG_ERROR(...)		Logger::getServiceLogger()->error("[{}:{}] {}", \
    fs::path(__FILE__).filename().string(), \
    __LINE__, \
    fmt::format(__VA_ARGS__))

#define GLOBAL_LOG_INFO(...)		Logger::getGlobalLogger()->info("[{}:{}] {}", \
    fs::path(__FILE__).filename().string(), \
    __LINE__, \
    fmt::format(__VA_ARGS__))
#define GLOBAL_LOG_WARN(...)		Logger::getGlobalLogger()->warn("[{}:{}] {}", \
    fs::path(__FILE__).filename().string(), \
    __LINE__, \
    fmt::format(__VA_ARGS__))
#define GLOBAL_LOG_DEBUG(...)		Logger::getGlobalLogger()->debug("[{}:{}] {}", \
    fs::path(__FILE__).filename().string(), \
    __LINE__, \
    fmt::format(__VA_ARGS__))
#define GLOBAL_LOG_ERROR(...)		Logger::getGlobalLogger()->error("[{}:{}] {}", \
    fs::path(__FILE__).filename().string(), \
    __LINE__, \
    fmt::format(__VA_ARGS__))

#define LOG_INFO(...) \
        SRV_LOG_INFO(__VA_ARGS__);\
        GLOBAL_LOG_INFO(__VA_ARGS__);

#define LOG_WARN(...) \
        SRV_LOG_WARN(__VA_ARGS__);\
        GLOBAL_LOG_WARN(__VA_ARGS__);

#define LOG_DEBUG(...) \
        SRV_LOG_DEBUG(__VA_ARGS__);\
        GLOBAL_LOG_DEBUG(__VA_ARGS__);

#define LOG_ERROR(...) \
        SRV_LOG_ERROR(__VA_ARGS__);\
        GLOBAL_LOG_ERROR(__VA_ARGS__);