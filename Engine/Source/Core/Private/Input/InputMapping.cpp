// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Input/InputMapping.h"
#include "JSON/JSON.h"
#include "Logger.h"

namespace ZeronEngine
{
	InputMapping::InputMapping()
		:m_InputMapFilePath("")
	{
		
		
	}

	InputMapping::InputMapping(const std::string& filePath)
	{
		ImportFromFile(filePath);
	}


	bool InputMapping::ImportFromFile(const std::string& filePath)
	{
		JSON json;
		const std::string absPath = absolute(std::filesystem::current_path() / std::filesystem::path(filePath)).string();
		if(JSON::ImportFromFile(json, absPath))
		{
			m_InputMapFilePath = filePath;
			ZERON_LOG_INFO("Importing input map from '{}'", absPath)

			// TODO Convert JSON Data to desired input structure	
			const JSONType& inputActionMappingType = json["InputActionMapping"];
			if(inputActionMappingType.IsValid())
			{
				const auto& inputActionMappingObject = inputActionMappingType.Get<JSONObject>();
				for(const auto& inputActionPair : inputActionMappingObject)
				{
					if(inputActionPair.second.IsValid())
					{
						const JSONArray& bindings = inputActionPair.second.Get<JSONArray>();
						//for(JSONType&& binding : bindings)
						//{
						//	const std::string& InputType = binding["InputType"].Get<std::string>();
						//	const std::string & InputName = binding["InputName"].Get<std::string>();

						//	//m_InputActionMap.emplace(inputActionPair.first, )
						//}
					}
				}
			}
			else
			{
				ZERON_LOG_ERROR("Input Action Mapping is not found!")
			}
			

			return true;
		}
		
		ZERON_LOG_ERROR("Failed to import input mapping from '{}'",std::filesystem::current_path() / filePath)

		return false;
	}

	bool InputMapping::ExportToFile(const std::string& filePath)
	{

		ZERON_LOG_ERROR("Failed to export input mapping from '{}'", std::filesystem::current_path() / filePath)

		return false;
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
