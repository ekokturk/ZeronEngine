// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Input/InputMapping.h"

#include "Logger.h"

namespace ZeronEngine
{
	InputMapping::InputMapping()
		:m_InputMapFilePath("")
	{
	}

	InputMapping::InputMapping(const std::string& filePath)
		:m_InputMapFilePath(filePath)
	{
		
	}

	InputMapping::~InputMapping()
	{
		
	}


	void InputMapping::ImportFromFile(const std::string& filePath)
	{
		m_InputMapFilePath = filePath;
	}

	void InputMapping::ExportToFile(const std::string& filePath)
	{
		
	}

	bool InputMapping::GetActionBinding(const std::string& bindingName, InputActionBinding& actionBinding) const
	{
		if(m_InputActionMap.count(bindingName) > 0)
		{
			actionBinding = m_InputActionMap.at(bindingName);
		}

		ZERON_LOG_WARNING("Action input binding '{}' is not found in input mapping.", bindingName)
		
		return false;
	}

	bool InputMapping::GetPollBinding(const std::string& bindingName, InputPollBinding& pollBinding) const
	{
		if (m_InputActionMap.count(bindingName) > 0)
		{
			pollBinding = m_InputPollMap.at(bindingName);
		}

		ZERON_LOG_WARNING("Poll input binding '{}' is not found in input mapping.", bindingName)

			return false;
	}
}
