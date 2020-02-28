#ifndef SLOG_H_
#define SLOG_H_

#include "include/spdlog/logger.h"
#include "include/spdlog/spdlog.h"

#include <memory>
#include <string>
#include <iostream>

namespace 
{
	const static uint32_t LogFileSize = 1024 * 1024 * 20; //20M 字节
	const static uint32_t MaxFileCount = 5; //最大日志文件数量
}

template <typename T>
class Singleton
{
public:
	static T& instance()
	{
		static T obj;

		creator.do_nothing();
		return obj;
	}
private:
	struct object_creator
	{
		object_creator() 
		{ 
			Singleton<T>::instance(); 
		}
		
		inline void do_nothing()const {}
	};

	static object_creator creator;
};

template <typename T> 
typename Singleton<T>::object_creator Singleton<T>::creator;

class SLog
{
public:
	SLog() {}
	~SLog() {}

	bool init(const std::string &log_name, spdlog::level::level_enum  level = spdlog::level::info)
	{
		return init(log_name,"log",level);
	};
	bool init(const std::string &logPath, const std::string &log_name, spdlog::level::level_enum level)
	{
		try {
			std::vector<spdlog::sink_ptr> sinks;
#ifdef WIN32
			sinks.push_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
#else
			sinks.push_back(std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>());
#endif
			sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logPath, LogFileSize, MaxFileCount));
			
			loger_ = std::make_shared<spdlog::async_logger>(log_name,begin(sinks), end(sinks), 1024, spdlog::async_overflow_policy::block_retry, nullptr, std::chrono::seconds(2));

#if _DEBUG
			loger_->set_level(spdlog::level::trace);
			loger_->flush_on(spdlog::level::trace);
#else
			loger_->set_level(level);
			loger_->flush_on(spdlog::level::critical);
#endif // _DEBUG
			spdlog::register_logger(loger_);
			spdlog::set_pattern("[%Y-%m-%d %T.%e] [%P:%t] [%n] [%l] %v");
		}
		catch (const spdlog::spdlog_ex& ex) {
			std::cout << "Log initialization failed: " << ex.what() << std::endl;
			return false;
		}

		return true;
	}
	void set_level(spdlog::level::level_enum level)
	{
#if _DEBUG
		loger_->set_level(spdlog::level::trace);
#else
		loger_->set_level(level);
#endif // _DEBUG
	}
	const std::shared_ptr<spdlog::logger>& getLoger() { return loger_; }

private:
	std::shared_ptr<spdlog::logger> loger_;
};

typedef Singleton<SLog> CSingletonSLog;

#define LOGGER (CSingletonSLog::instance().getLoger())

#define LOG_TRACE(...)		(LOGGER->trace(__VA_ARGS__))
#define LOG_DEBUG(...)		(LOGGER->debug(__VA_ARGS__))
#define LOG_INFO(...)		(LOGGER->info(__VA_ARGS__))
#define LOG_WARN(...)		(LOGGER->warn(__VA_ARGS__))
#define LOG_ERROR(...)		(LOGGER->error(__VA_ARGS__))
#define LOG_CRITICAL(...)	(LOGGER->critical(__VA_ARGS__))

#endif // !SLOG_H_
