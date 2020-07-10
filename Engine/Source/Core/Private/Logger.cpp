// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Logger.h"

namespace ZeronEngine
{
	std::unique_ptr<Logger> Logger::s_Instance;
	
	Logger::Logger()
		:m_Time(std::time(nullptr)),
		 m_Format("[{t:%Y-%m-%d | %H:%M:%S}]{p} {m}\n")
	{
		m_LogFile.open("Log.txt", std::ofstream::out | std::ofstream::trunc);
		AddLogPrefix((ELogType::Info), " - [INFO] -");
		AddLogPrefix(ELogType::Warning, " - [WARNING] -");
		AddLogPrefix(ELogType::Error, " - [ERROR] -");
		AddLogPrefix(ELogType::Fatal, " - [FATAL] -");
	}

	Logger::~Logger()
	{
		ZERON_LOG_INFO("Logger shutting down.")
		m_LogFile.close();
	}

	void Logger::Init()
	{
		s_Instance = std::unique_ptr<Logger>(new Logger());
		ZERON_LOG_INFO("Logger initialized.")
	}

	Logger *  Logger::GetInstance()
	{
		return s_Instance.get();
	}

	void Logger::AddLogPrefix(ELogType id, const std::string& prefix)
	{
		if(m_TypePrefixes.find(id) == m_TypePrefixes.end())
		{
			m_TypePrefixes.insert(std::make_pair(id, prefix));
		}
	}
}
