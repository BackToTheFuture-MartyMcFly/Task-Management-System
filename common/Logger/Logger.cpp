#include <Logger.h>

std::string Logger::m_service_name = "";
std::string Logger::m_log_dir = "\\var\\log";
std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> Logger::m_service_loggers;
std::shared_ptr<spdlog::logger> Logger::m_global_logger = nullptr;
bool Logger::m_initialize = false;


void Logger::init(const std::string& service_name, std::string log_dir, spdlog::level::level_enum log_level)
{
	if (m_initialize) {
		// сообщение о логах
		return;
	}

	m_service_name = service_name;
	m_log_dir = log_dir;

	auto absolute_path = std::filesystem::absolute("..\\..");
	m_log_dir = absolute_path.string() + m_log_dir;

	if (!std::filesystem::exists(m_log_dir))
		std::filesystem::create_directories(m_log_dir);
	
	createGlobalLogger();

	auto logger = createServiceLogger(m_service_name);

	setLevel(log_level); //Вынести в конфигурацию

	m_initialize = true;
}

std::shared_ptr<spdlog::logger> Logger::getServiceLogger(const std::string& service_name) {
	if (!m_initialize)
		throw std::runtime_error("Logger not initialize");

	auto name = service_name.empty() ? m_service_name : service_name;

	auto it = m_service_loggers.find(name);
	
	if (it != m_service_loggers.end())
		return it->second;

	return createServiceLogger(service_name);
}

std::shared_ptr<spdlog::logger> Logger::getGlobalLogger() {
	if(!m_initialize)
		throw std::runtime_error("Logger not initialize");

	return m_global_logger;
}

void Logger::createGlobalLogger() {
		
	std::vector<spdlog::sink_ptr> sinks;
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	sinks.push_back(console_sink);
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(m_log_dir + "\\global.log");
	sinks.push_back(file_sink);

	m_global_logger = std::make_shared<spdlog::logger>("GLOBAL", sinks.begin(), sinks.end());

	m_global_logger->set_level(spdlog::level::info);
	m_global_logger->flush_on(spdlog::level::warn);

	spdlog::register_logger(m_global_logger);

}

std::shared_ptr<spdlog::logger> Logger::createServiceLogger(const std::string& service_name) {
	
	auto it = m_service_loggers.find(service_name);
	if (it != m_service_loggers.end())
		return it->second;

	std::vector<spdlog::sink_ptr> sinks;
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	sinks.push_back(console_sink);
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_st>(m_log_dir + "\\" + service_name + ".log");
	sinks.push_back(file_sink);

	auto service_logger = std::make_shared<spdlog::logger>(service_name, sinks.begin(), sinks.end());
	service_logger->flush_on(spdlog::level::err);

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
		
	if (m_global_logger)
		m_global_logger->set_level(level);
}


void Logger::shutdown() {
	
	for (auto& [srvName, logger] : m_service_loggers) {
		if (logger)
			logger->flush();
	}

	if (m_global_logger)
		m_global_logger->flush();

	spdlog::shutdown();
	m_initialize = false;

}