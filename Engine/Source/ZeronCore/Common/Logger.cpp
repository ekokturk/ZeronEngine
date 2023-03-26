// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Common/Logger.h>

#include <Common/Time.h>
#include <fmt/color.h>
#include <fmt/printf.h>

#if ZE_PLATFORM_ANDROID
#include <android/log.h>
#endif

namespace Zeron
{
	namespace {
	#if ZE_PLATFORM_ANDROID
		void FlushToAndroidConsole(const std::string& message, Color color) {
			android_LogPriority priority = ANDROID_LOG_INFO;
			if (color == Color::Red) {
				priority = android_LogPriority::ANDROID_LOG_ERROR;
			}
			else if (color == Color::Yellow) {
				priority = android_LogPriority::ANDROID_LOG_WARN;
			}
			__android_log_print(priority, "Zeron Engine", "%s", message.c_str());
		}
	#endif
	}

	Logger::Logger(bool shouldTimestamp)
		: mShouldTimestamp(shouldTimestamp)
	{
#if ZE_PLATFORM_WIN32
		// Enable console text colors on Windows
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD dwMode = 0;
		GetConsoleMode(hOut, &dwMode);
		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(hOut, dwMode);
#endif
	}

	bool Logger::InitLogFile(std::string_view logFilePath)
	{
		if(mLogFile.is_open()) {
			return false;
		}
		
		if (std::filesystem::exists(logFilePath)) {
			if (!std::filesystem::remove(logFilePath)) {
				FlushToConsole("ERROR: Couldn't modify log file!", Color::Red);
				return false;
			}
		}
#if ZE_PLATFORM_WIN32
		mLogFile.open(logFilePath, std::ofstream::out | std::ofstream::ios_base::app);
#endif
		return true;
	}

	std::string Logger::GetMessageWithTimeStamp(const std::string& message) const
	{
		const Time::TimeStamp localTime = Time::GetLocalTime();
		return Util::Format("[{:02}:{:02}:{:02}] {}", localTime.CountHours(), localTime.CountMinutes(), localTime.CountSeconds(), message);
	}

	void Logger::FlushToFile(const std::string& message)
	{
		if (mLogFile.is_open()) {
			mLogFile << message;
			mLogFile.flush();
		}
	}

	void Logger::FlushToConsole(const std::string& message, Color color) const
	{
	#if ZE_PLATFORM_ANDROID
		FlushToAndroidConsole(message, color);
	#else
		fmt::print(fmt::fg(static_cast<fmt::color>(color.HexRGB())), message.c_str());
	#endif
	}
}

