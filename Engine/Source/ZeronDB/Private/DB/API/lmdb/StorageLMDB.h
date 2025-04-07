// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <DB/MapStorage.h>
#include <lmdb.h>


namespace Zeron::DB
{
	class StorageLMDB : public MapStorage {
	  public:
		StorageLMDB(const MapStorageConfig& config);
		~StorageLMDB();

		std::unique_ptr<Transaction> CreateTransaction(std::string_view db, bool readOnly) override;
		void PutValue(const std::string& key, const std::string& value, std::string_view db) override;
		std::string GetValue(const std::string& key, std::string_view db) override;
		void DeleteValue(const std::string& key, std::string_view db) override;

		bool CopyStorage(const Path& path) override;

		// LMDB
		MDB_env* GetEnv() const;


	  private:
		MDB_env* mEnvironment;
	};
}