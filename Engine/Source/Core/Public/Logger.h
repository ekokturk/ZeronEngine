// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.
#pragma once

#include <string>
#include <memory>
#include <fstream>
#include "Color.h"

// FMT library for string formating
// Disable fmt library warning
#pragma warning( disable : 4003)
#include "fmt/core.h"
#include "fmt/color.h"
#include "fmt/chrono.h"
#include "fmt/printf.h"
#include <map>

#define ZERON_LOG(...)							ZeronEngine::Logger::GetInstance()->Log(__VA_ARGS__);
#define ZERON_LOG_INFO(...)						ZeronEngine::Logger::GetInstance()->Log(ELogType::Info,		EColor::Gray, __VA_ARGS__);
#define ZERON_LOG_WARNING(...)					ZeronEngine::Logger::GetInstance()->Log(ELogType::Warning,	EColor::Yellow, __VA_ARGS__);
#define ZERON_LOG_ERROR(...)					ZeronEngine::Logger::GetInstance()->Log(ELogType::Error,	EColor::Red, __VA_ARGS__);
#define ZERON_LOG_FATAL(...)					ZeronEngine::Logger::GetInstance()->Log(ELogType::Fatal,	EColor::Purple, __VA_ARGS__);
#define ZERON_LOGC(Type, Color, Message, ...)	ZeronEngine::Logger::GetInstance()->Log(ELogType::Type,		EColor::Color, Message, __VA_ARGS__);

namespace ZeronEngine
{
	/// Log Type that defines message prefix and log level
	enum class ELogType : uint8_t
	{
		Default		= 0,
		Info,		
		Warning,		
		Error,		
		Fatal,		
	};

	class Logger
	{
	private:
		Logger();
		static std::unique_ptr<Logger> s_Instance;
		
		std::time_t m_Time;										// Time pointer for time stamp
		const char* m_Format;									// Format of the log message
		std::ofstream m_LogFile;								// Output file of Log
		std::map<ELogType, const std::string> m_TypePrefixes;	// Prefix for a message for log types
		
	public:
		Logger(Logger const&) = delete;
		void operator=(Logger const&) = delete;
		~Logger();

		/// Initialize logger instance
		static void Init();

		/// Return logger instance
		static Logger * GetInstance();

		/// Log message
		template<typename... Args>
		void Log(const char* fmt, Args... args)
		{
			const std::string format = GetLogFormat(fmt);
			fmt::print(format, args...);
		}
		
		/// Log message with color
		template<typename... Args>
		void Log(EColor color, const char* fmt, Args... args)
		{
			const std::string format = GetLogFormat(fmt);
			fmt::print(fg(fmt::rgb(static_cast<fmt::color>(color))), format, args...);
		}

		/// Log message with color and type
		template<typename... Args>
		void Log(ELogType logType, EColor color, const char* fmt, Args... args)
		{
			const std::string format = GetLogFormat(fmt, m_TypePrefixes[logType].c_str());
			fmt::print(fg(fmt::rgb(static_cast<fmt::color>(color))), format, args...);
			fmt::print(m_LogFile, format, args...);
		}

		void AddLogPrefix(ELogType id, std::string prefix);

	private:
		/// Format log string with timestamp and message
		inline std::string GetLogFormat(const char* fmt, const char* messagePrefix = "") const
		{
			return fmt::format(m_Format, fmt::arg("t", fmt::localtime(m_Time)), 
						fmt::arg("p", messagePrefix), fmt::arg("m", fmt));
		}
	};

}



