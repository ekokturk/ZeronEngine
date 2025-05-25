// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <DB/Transaction.h>

namespace Zeron::DB
{
	struct MapStorageConfig {
		std::string mPath;
		size_t mSize = 0;
	};


	// Represents a key-value pair database
	class MapStorage {
	  public:
		static std::unique_ptr<MapStorage> Create(const MapStorageConfig& config);

		virtual ~MapStorage() = default;

		virtual std::unique_ptr<Transaction> CreateTransaction(std::string_view db = "", bool readOnly = false) = 0;
		virtual void PutValue(const std::string& key, const std::string& value, std::string_view db = "") = 0;
		virtual std::string GetValue(const std::string& key, std::string_view db = "") = 0;
		virtual void DeleteValue(const std::string& key, std::string_view db = "") = 0;
		virtual bool CopyStorage(const std::string& path) = 0;
	};
}