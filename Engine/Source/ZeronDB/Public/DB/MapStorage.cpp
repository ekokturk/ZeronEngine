// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <DB/MapStorage.h>

#include <DB/API/lmdb/StorageLMDB.h>


namespace Zeron::DB
{
	std::unique_ptr<MapStorage> MapStorage::Create(const MapStorageConfig& config)
	{
		return std::make_unique<StorageLMDB>(config);
	}
}