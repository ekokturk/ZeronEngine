// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Common/Util.h>
#include <Common/Types/Color.h>
#include <Common/Types/Enum.h>

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
		Logger(bool shouldTimestamp);
		virtual ~Logger() = default;

		bool InitLogFile(std::string_view logFilePath);
		
		template<typename... Args>
		void Log(LogSink logSink, Color color, std::string_view message, Args&&... args)
		{
			const std::string msg = FormatLogMessage(message, std::forward<Args>(args)...);
			if (!(logSink & LogSink::Console)) {
				FlushToConsole(msg, color);
			}
			if (!(logSink & LogSink::File)) {
				FlushToFile(msg);
			}
		}

		template<typename... Args>
		void Log(std::string_view message, Args&&... args)
		{
			Log(LogSink::FileAndConsole, Color::White, message, std::forward<Args>(args)...);
		}

	protected:
		template<typename... Args>
		std::string FormatLogMessage(std::string_view message, Args&&... args) const
		{
			const std::string formatted = Util::Format(message, std::forward<Args>(args)...) + "\n";
			if (mShouldTimestamp) {
				return GetMessageWithTimeStamp(formatted);
			}
			return formatted;
		}

		std::string GetMessageWithTimeStamp(const std::string& message) const;
		void FlushToFile(const std::string& message);
		void FlushToConsole(const std::string& message, Color color) const;

		std::ofstream mLogFile;
		bool mShouldTimestamp;
	};

	static Logger& ZELogger() {
		static Logger logger(true);
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