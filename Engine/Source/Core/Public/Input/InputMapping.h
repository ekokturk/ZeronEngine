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

		// Import input mappings from file (Overwrite existing)
		void ImportFromFile(const std::string& filePath);

		// Export input mappings to file
		void ExportToFile(const std::string& filePath);


		bool GetActionBinding(const std::string& bindingName, InputActionBinding& actionBinding) const;

		bool GetPollBinding(const std::string& bindingName, InputPollBinding& pollBinding) const;

	private:
		std::unordered_map<std::string, InputActionBinding>	m_InputActionMap;
		std::unordered_map<std::string, InputPollBinding>	m_InputPollMap;

		std::string m_InputMapFilePath;
	};
}

