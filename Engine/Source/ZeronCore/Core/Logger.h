// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "Types/Color.h"
#include "Types/Enum.h"
#pragma warning( disable : 4003)
#include "fmt/core.h"
#include "fmt/color.h"
#include "fmt/chrono.h"
#include "fmt/printf.h"

namespace Zeron {
	enum LogSink
	{
		None = 0,
		Console = 1 << 1,
		File = 1 << 2,
		FileAndConsole = File | Console,
	};

	ZE_ENUM_OPERATIONS(LogSink)

	class Logger {
	public:
		Logger(std::string_view logFilePath, bool shouldTimestamp);

		template<typename... Args>
		void Log(LogSink logSink, Color color, const char* message, Args... args)
		{
			const std::string msg = FormatLogMessage(message, args...);
			if (!(logSink & LogSink::Console)) {
				FlushToConsole(msg, color);
			}
			if (!(logSink & LogSink::File)) {
				FlushToFile(msg);
			}
		}

		template<typename... Args>
		void Log(const char* message, Args... args)
		{
			Log(LogSink::ConsoleAndFile, Color::White, message, std::forward<Args>(args)...);
		}

	protected:
		template<typename... Args>
		std::string FormatLogMessage(const char* fmt, Args... args) const
		{
			std::string message = fmt::format(fmt, args...) + "\n";
			if (mShouldTimestamp) {
				message = GetTimeStamp() + message;
			}
			return message;
		}

		std::string GetTimeStamp() const;
		void FlushToFile(const std::string& message);
		void FlushToConsole(const std::string& message, Color color);

		std::ofstream mLogFile;
		bool mShouldTimestamp;
	};

	static Logger& ZELogger() {
		static Logger logger("log.txt", true);
		return logger;
	}
}

#ifndef ZERON_LOGGER_DISABLED
	#define ZE_LOG(...)								::Zeron::ZELogger().Log(::Zeron::LogSink::Console,		  ::Zeron::Color::White,	__VA_ARGS__)
	#define ZE_LOGI(...)							::Zeron::ZELogger().Log(::Zeron::LogSink::FileAndConsole, ::Zeron::Color::Gray,		__VA_ARGS__)
	#define ZE_LOGW(...)							::Zeron::ZELogger().Log(::Zeron::LogSink::FileAndConsole, ::Zeron::Color::Yellow,	__VA_ARGS__)
	#define ZE_LOGE(...)							::Zeron::ZELogger().Log(::Zeron::LogSink::FileAndConsole, ::Zeron::Color::Red,		__VA_ARGS__)
	#define ZE_LOGC(Sink, Color, Message, ...)		::Zeron::ZELogger().Log(Sink, Color, Message, __VA_ARGS)
#else
	#define ZE_LOG(...)	
	#define ZE_LOGI(...)
	#define ZE_LOGW(...)
	#define ZE_LOGE(...)
	#define ZE_LOGC(Sink, Color, Message, ...)
#endif