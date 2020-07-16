// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Input/InputMapping.h"

namespace ZeronEngine
{
	InputMapping::InputMapping()
		:m_InputMapFilePath("")
	{
		// TODO Remove after testing
		m_MouseMapping["TestInput"] = MouseCode::LeftButton;
	}

	InputMapping::InputMapping(const std::string& filePath)
		:m_InputMapFilePath(filePath)
	{
		
	}

	InputMapping::~InputMapping()
	{
		
	}


	MouseCode InputMapping::GetMouseCode(const std::string& inputName) const
	{
		if(m_MouseMapping.count(inputName) > 0)
		{
			return m_MouseMapping.at(inputName);
		}

		return MouseCode::Unknown;
	}

	KeyCode InputMapping::GetKeyCode(const std::string& inputName) const
	{
		
		return KeyCode::Unknown;
	}

	void InputMapping::ImportFromFile(const std::string& filePath)
	{
		m_InputMapFilePath = filePath;
	}

	void InputMapping::ExportToFile(const std::string& filePath)
	{
		
	}
}
