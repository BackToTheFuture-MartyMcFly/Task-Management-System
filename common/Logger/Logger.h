#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <unordered_map>
#include <memory>
#include <filesystem>
#include <mutex>

namespace fs = std::filesystem;

class Logger {
	private:
        static std::mutex m_mutex;
		static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> m_service_loggers;
        //static std::shared_ptr<spdlog::logger> m_global_logger;
		//static bool m_initialize_global;

public:
	static void init(const std::string& service_name, spdlog::level::level_enum log_level = spdlog::level::info);
	static std::shared_ptr<spdlog::logger> getServiceLogger(const std::string& service_name = "");
	//static std::shared_ptr<spdlog::logger> getGlobalLogger();
	static std::shared_ptr<spdlog::logger> createServiceLogger(const std::string& service_name);
    //static void createGlobalLogger();
	static void setLevel(spdlog::level::level_enum level);
	static void shutdown();

};

#define LOG_INIT(...)           Logger::init(__VA_ARGS__)
#define LOG_SET_LEVEL(level)    Logger::setLevel(level)
#define LOG_SHUTDOWN()          Logger::shutdown()

/*#define GLOBAL_LOG_INFO(...)		Logger::getGlobalLogger()->info("[{}:{}] {}", \
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
    fmt::format(__VA_ARGS__))*/

#define SRV_LOG(service, level, ...) \
    do { \
		auto logger = Logger::getServiceLogger(#service); \
        if(logger) { \
            logger->level("[{}:{}] {}",\
            fs::path(__FILE__).filename().string(), \
             __LINE__, \
            fmt::format(__VA_ARGS__)); \
        } \
    } while(0);

#define SRV_LOG_INFO(service, ...)  SRV_LOG(service, info, __VA_ARGS__)
#define SRV_LOG_WARN(service, ...)  SRV_LOG(service, warn, __VA_ARGS__)
#define SRV_LOG_ERROR(service, ...) SRV_LOG(service, error, __VA_ARGS__)
#define SRV_LOG_DEBUG(service, ...) SRV_LOG(service, debug, __VA_ARGS__)
#define SRV_LOG_TRACE(service, ...) SRV_LOG(service, trace, __VA_ARGS__)

/*#define LOG_INFO(service, ...) \
        SRV_LOG_INFO(service, __VA_ARGS__);\
        //GLOBAL_LOG_INFO(__VA_ARGS__);

#define LOG_WARN(service, ...) \
        SRV_LOG_WARN(service, __VA_ARGS__);\
        //GLOBAL_LOG_WARN(__VA_ARGS__);

#define LOG_DEBUG(service, ...) \
        SRV_LOG_DEBUG(service, __VA_ARGS__);\
        //GLOBAL_LOG_DEBUG(__VA_ARGS__);

#define LOG_ERROR(service, ...) \
        SRV_LOG_ERROR(service, __VA_ARGS__);\
        //GLOBAL_LOG_ERROR(__VA_ARGS__);*/

#ifndef SERVICE_NAME
#define SERVICE_NAME ""
#endif

#define LOG_INFO(...) \
        SRV_LOG_INFO(SERVICE_NAME, __VA_ARGS__);\
        //GLOBAL_LOG_INFO(__VA_ARGS__);

#define LOG_WARN(...) \
        SRV_LOG_WARN(SERVICE_NAME, __VA_ARGS__);\
        //GLOBAL_LOG_WARN(__VA_ARGS__);

#define LOG_DEBUG(...) \
        SRV_LOG_DEBUG(SERVICE_NAME, __VA_ARGS__);\
        //GLOBAL_LOG_DEBUG(__VA_ARGS__);

#define LOG_ERROR(...) \
        SRV_LOG_ERROR(SERVICE_NAME, __VA_ARGS__);\
        //GLOBAL_LOG_ERROR(__VA_ARGS__);