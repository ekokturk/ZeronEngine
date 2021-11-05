// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Logger.h"

namespace Zeron
{
	Logger::Logger(std::string_view logFilePath, bool shouldTimestamp)
		: mShouldTimestamp(shouldTimestamp)
	{
		if (std::filesystem::exists(logFilePath)) {
			if (!std::filesystem::remove(logFilePath)) {
				FlushToConsole("ERROR: Couldn't modify log file!", Color::Red);
				return;
			}
		}
		mLogFile.open(logFilePath, std::ofstream::out | std::ofstream::ios_base::app);
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
		mLogFile << message;
		mLogFile.flush();
	}

	void Logger::FlushToConsole(const std::string& message, Color color)
	{
		fmt::print(fmt::fg(static_cast<fmt::color>(color.HexRGB())), message.c_str());
	}
}

