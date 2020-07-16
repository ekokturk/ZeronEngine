// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "InputTypes.h"

namespace ZeronEngine
{
	class InputMapping
	{
	public:
		InputMapping();
		InputMapping(const std::string& filePath);
		~InputMapping();

		MouseCode	GetMouseCode(const std::string& inputName) const;
		KeyCode		GetKeyCode(const std::string& inputName) const;

		// Import input mappings from file (Overwrite existing)
		void ImportFromFile(const std::string& filePath);

		// Export input mappings to file
		void ExportToFile(const std::string& filePath);
		
	private:
		std::unordered_map<std::string, MouseCode>	m_MouseMapping;
		std::unordered_map<std::string, KeyCode>	m_KeyMapping;

		std::string m_InputMapFilePath;
	};
}
