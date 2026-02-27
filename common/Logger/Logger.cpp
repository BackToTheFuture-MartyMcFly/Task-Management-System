#include <Logger.h>
#include <iostream>

std::mutex Logger::m_mutex;
std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> Logger::m_service_loggers;
//std::shared_ptr<spdlog::logger> Logger::m_global_logger = nullptr;
//bool Logger::m_initialize_global_log = false;


void Logger::init(const std::string& service_name, spdlog::level::level_enum log_level)
{
	/*if (!m_initialize_global_log) {
		createGlobalLogger();
		m_initialize_global_log = true;
	}*/

	if (service_name.empty()) {
		throw std::invalid_argument("Service name cannot be empty in createServiceLogger");
	}
	
	createServiceLogger(service_name);

	setLevel(log_level); //in future add configuration
}

std::shared_ptr<spdlog::logger> Logger::getServiceLogger(const std::string& service_name) {

	auto it = m_service_loggers.find(service_name);
	if (it != m_service_loggers.end())
		return it->second;

	return createServiceLogger(service_name);
}

/*std::shared_ptr<spdlog::logger> Logger::getGlobalLogger() {
	if(!m_initialize_global_log)
		throw std::runtime_error("Global logger not initialize");

	return m_global_logger;
}

void Logger::createGlobalLogger() {
		
	/*std::vector<spdlog::sink_ptr> sinks;
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	sinks.push_back(console_sink);
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>( "../global.log");
	sinks.push_back(file_sink);

	m_global_logger = std::make_shared<spdlog::logger>("GLOBAL", sinks.begin(), sinks.end());
	m_global_logger->set_pattern("[%Y-%m-%d %T] [%^%l%$] %v");
	m_global_logger->flush_on(spdlog::level::err);
	
	spdlog::register_logger(m_global_logger);

}*/

std::shared_ptr<spdlog::logger> Logger::createServiceLogger(const std::string& service_name) {
	std::lock_guard<std::mutex> lock(m_mutex);
	auto it = m_service_loggers.find(service_name);
	if (it != m_service_loggers.end())
		return it->second;

	std::vector<spdlog::sink_ptr> sinks;
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	sinks.push_back(console_sink);
	fs::path path = "../logs/" + service_name + ".log";
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_st>(path.string());
	sinks.push_back(file_sink);

	auto service_logger = std::make_shared<spdlog::logger>(service_name, sinks.begin(), sinks.end());
	service_logger->set_pattern("[%Y-%m-%d %T] [%^%l%$] %v");
	service_logger->flush_on(spdlog::level::info);

	spdlog::register_logger(service_logger);

	m_service_loggers.insert(std::make_pair(service_name, service_logger));

	return service_logger;
}

void Logger::setLevel(spdlog::level::level_enum level) {
	spdlog::set_level(level);

	for (auto& [srvName, logger] : m_service_loggers) {
		if(logger)
			logger->set_level(level);
	}
		
	/*if (m_global_logger)
		m_global_logger->set_level(level);*/
}

void Logger::shutdown() {
	
	for (auto& [srvName, logger] : m_service_loggers) {
		if (logger)
			logger->flush();
	}
	
	/*if (m_global_logger)
		m_global_logger->flush();*/

	m_service_loggers.clear();

	spdlog::shutdown();
	
	//m_initialize_global_log = false;

}