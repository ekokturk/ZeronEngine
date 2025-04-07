// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <DB/MapStorage.h>
#include <lmdb.h>


namespace Zeron::DB
{
	class StorageLMDB;

	class TransactionLMDB : public Transaction {
	  public:
		TransactionLMDB(StorageLMDB& storage, std::string_view db, bool readOnly);
		~TransactionLMDB() override;

		void Commit() override;
		void Discard() override;
		void Put(const std::string& key, const std::string& value) override;
		std::string Get(const std::string& key) override;
		void Delete(const std::string& key) override;

	  private:
		void _discard();

		MDB_txn* mTransaction;
		MDB_dbi mDBHandle;
		bool mIsCommitted;
	};
}