// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <DB/API/lmdb/TransactionLMDB.h>

#include <DB/API/lmdb/StorageLMDB.h>

namespace Zeron::DB
{

	TransactionLMDB::TransactionLMDB(StorageLMDB& storage, std::string_view db, bool readOnly)
		: mTransaction(nullptr)
		, mDBHandle(-1)
		, mIsCommitted(false)
	{
		if (mdb_txn_begin(storage.GetEnv(), nullptr, readOnly ? MDB_RDONLY : 0, &mTransaction) != 0) {
			throw std::runtime_error("Failed to begin LMDB transaction");
		}
		const int result = mdb_dbi_open(mTransaction, db.empty() ? nullptr : db.data(), MDB_CREATE, &mDBHandle);
		if (result != MDB_SUCCESS) {
			ZE_FAIL("Failed to open LMDB database. {}", mdb_strerror(result));
			throw std::runtime_error("Failed to open LMDB database");
		}
	}

	TransactionLMDB::~TransactionLMDB()
	{
		if (!mIsCommitted) {
			_discard();
		}
		// TODO: This should probably be handled by the StorageLMDB, consider changing the API
		// mdb_dbi_close(mdb_txn_env(mTransaction), mDBHandle);
	}
	void TransactionLMDB::Commit()
	{
		if (mdb_txn_commit(mTransaction) == 0) {
			mIsCommitted = true;
		}
		else {
			throw std::runtime_error("Failed to commit transaction");
		}
	}

	void TransactionLMDB::Discard()
	{
		_discard();
	}

	void TransactionLMDB::Put(const std::string& key, const std::string& value)
	{
		MDB_val k{ key.size(), (void*)key.data() };
		MDB_val v{ value.size(), (void*)value.data() };
		if (mdb_put(mTransaction, mDBHandle, &k, &v, 0) != 0) {
			// TODO: Convert this to result
			throw std::runtime_error("Failed to put data");
		}
	}

	std::string TransactionLMDB::Get(const std::string& key)
	{
		MDB_val k{ key.size(), (void*)key.data() };
		MDB_val v;
		if (mdb_get(mTransaction, mDBHandle, &k, &v) == 0) {
			return std::string(static_cast<char*>(v.mv_data), v.mv_size);
		}

		// TODO: Convert this to result
		return "";
	}

	void TransactionLMDB::Delete(const std::string& key)
	{
		MDB_val k{ key.size(), (void*)key.data() };
		if (mdb_del(mTransaction, mDBHandle, &k, nullptr) != 0) {
			// TODO: Convert this to result
			throw std::runtime_error("Failed to delete key");
		}
	}

	void TransactionLMDB::_discard()
	{
		mdb_txn_abort(mTransaction);
	}
}