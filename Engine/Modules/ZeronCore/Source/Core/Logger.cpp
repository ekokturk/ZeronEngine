// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Core/Logger.h>

namespace Zeron
{
	Logger::Logger(bool shouldTimestamp)
		: mShouldTimestamp(shouldTimestamp)
	{
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

	std::string Logger::GetTimeStamp() const 
	{
		std::time_t time;
		std::time(&time);
		const char* format = "[{t:" /*"%Y-%m-%d | "*/ "%H:%M:%S" "}] ";
		return fmt::format(format, fmt::arg("t", fmt::localtime(time)));
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
		fmt::print(fmt::fg(static_cast<fmt::color>(color.HexRGB())), message.c_str());
	}
}

