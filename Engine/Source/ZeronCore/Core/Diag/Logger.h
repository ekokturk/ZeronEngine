// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Core/Types/Color.h>
#include <Core/Types/Enum.h>
#include <Core/Util.h>

namespace Zeron
{
	enum class LogSink
	{
		None = 0,
		Console = 1 << 1,
		File = 1 << 2,
		FileAndConsole = File | Console,
	};

	ZE_ENUM_OPERATIONS(LogSink)

	enum class LogFlags
	{
		None = 0,
		TimeStamp = 1 << 0
	};

	ZE_ENUM_OPERATIONS(LogFlags)
#define ZE_LOG_DEFAULT_FLAGS LogFlags::TimeStamp

	class Logger {
	  public:
		Logger();
		virtual ~Logger() = default;

		bool InitLogFile(std::string_view logFilePath);

		void UpdateFlags(LogFlags flags) { mFlags = flags; }

		template <typename... Args>
		void Log(LogSink logSink, Color color, Args&&... args)
		{
			if constexpr (sizeof...(Args) != 0) {
				const std::string msg = _formatLogMessage(std::forward<Args>(args)...);
				if (!(logSink & LogSink::Console)) {
					_flushToConsole(msg, color);
				}
				if (!(logSink & LogSink::File)) {
					_flushToFile(msg);
				}
			}
		}

		template <typename... Args>
		void Log(std::string_view message, Args&&... args)
		{
			Log(LogSink::FileAndConsole, Color::White, message, std::forward<Args>(args)...);
		}

	  protected:
		template <typename... Args>
		std::string _formatLogMessage(std::string_view message, Args&&... args) const
		{
			const std::string formatted = Util::Format(message, std::forward<Args>(args)...) + "\n";
			if (!(mFlags & LogFlags::TimeStamp)) {
				return _getMessageWithTimeStamp(formatted);
			}
			return formatted;
		}

		std::string _getMessageWithTimeStamp(const std::string& message) const;
		void _flushToFile(const std::string& message);
		void _flushToConsole(const std::string& message, Color color) const;

		std::ofstream mLogFile;
		LogFlags mFlags;
	};

	Logger& GlobalLogger();
}

#ifndef ZERON_LOGGER_DISABLED
#	define ZE_LOG_FILE(...)				   ::Zeron::GlobalLogger().InitLogFile(__VA_ARGS__)
#	define ZE_LOG(...)						   ::Zeron::GlobalLogger().Log(::Zeron::LogSink::Console, ::Zeron::Color::Gray __VA_OPT__(, ) __VA_ARGS__)
#	define ZE_LOGI(...)					   ::Zeron::GlobalLogger().Log(::Zeron::LogSink::FileAndConsole, ::Zeron::Color::White __VA_OPT__(, ) __VA_ARGS__)
#	define ZE_LOGW(...)					   ::Zeron::GlobalLogger().Log(::Zeron::LogSink::FileAndConsole, ::Zeron::Color::Yellow __VA_OPT__(, ) __VA_ARGS__)
#	define ZE_LOGE(...)					   ::Zeron::GlobalLogger().Log(::Zeron::LogSink::FileAndConsole, ::Zeron::Color::Red __VA_OPT__(, ) __VA_ARGS__)
#	define ZE_LOGC(Sink, Color, Message, ...) ::Zeron::GlobalLogger().Log(Sink, Color, Message __VA_OPT__(, ) __VA_ARGS)
#else
#	define ZE_LOG_CONFIG(...)
#	define ZE_LOG(...)
#	define ZE_LOGI(...)
#	define ZE_LOGW(...)
#	define ZE_LOGE(...)
#	define ZE_LOGC(Sink, Color, Message, ...)
#endif