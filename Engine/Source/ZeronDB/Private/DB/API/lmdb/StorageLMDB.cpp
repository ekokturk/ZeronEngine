// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <DB/API/lmdb/StorageLMDB.h>
#include <DB/API/lmdb/TransactionLMDB.h>
#include <lmdb.h>


namespace Zeron::DB
{

	StorageLMDB::StorageLMDB(const MapStorageConfig& config)
	{
		if (mdb_env_create(&mEnvironment) != 0) {
			throw std::runtime_error("Failed to create LMDB environment");
		}
		ZE_ASSERT(config.mSize > 0, "Expected to have an initial LMDB storage size");

		mdb_env_set_mapsize(mEnvironment, config.mSize);

		mdb_env_set_maxdbs(mEnvironment, 512);

		// TODO: Investigate best env flags for use
		const int envFlags = MDB_NOSYNC | MDB_WRITEMAP;
		const int result = mdb_env_open(mEnvironment, config.mPath.ToString().c_str(), envFlags, 0664);
		if (result != MDB_SUCCESS) {
			ZE_FAIL("Failed to open LMDB environment in {}. {}", config.mPath.ToString().c_str(), mdb_strerror(result));
			throw std::runtime_error("Failed to open LMDB environment");
		}
	}

	StorageLMDB::~StorageLMDB()
	{
		mdb_env_close(mEnvironment);
	}

	std::unique_ptr<Transaction> StorageLMDB::CreateTransaction(std::string_view db, bool readOnly)
	{
		return std::make_unique<TransactionLMDB>(*this, db, readOnly);
	}

	void StorageLMDB::PutValue(const std::string& key, const std::string& value, std::string_view db)
	{
		TransactionLMDB t(*this, db, false);
		t.Put(key, value);
		t.Commit();
	}

	std::string StorageLMDB::GetValue(const std::string& key, std::string_view db)
	{

		TransactionLMDB t(*this, db, true);
		std::string val = t.Get(key);
		t.Commit();
		return val;
	}

	void StorageLMDB::DeleteValue(const std::string& key, std::string_view db)
	{
		TransactionLMDB t(*this, db, false);
		t.Delete(key);
		t.Commit();
	}

	bool StorageLMDB::CopyStorage(const Path& path)
	{
		if (mdb_env_copy(mEnvironment, path.ToString().c_str()) != MDB_SUCCESS) {
			return false;
		}
		return true;
	}

	MDB_env* StorageLMDB::GetEnv() const
	{
		return mEnvironment;
	}
}