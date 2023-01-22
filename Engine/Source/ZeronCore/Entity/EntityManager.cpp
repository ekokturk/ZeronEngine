// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Entity/EntityManager.h>

namespace Zeron
{
	Entity EntityManager::CreateEntity()
	{
		Entity newEntity{ static_cast<uint32_t>(mEntityPool.size()) };
		if (mFreeEntities.empty()) {
			ZE_ASSERT(newEntity.GetId() == mEntityPool.size(), "Entity id should match the stored index");
			mEntityPool.push_back(newEntity);
		}
		else {
			newEntity = mFreeEntities.back();
			newEntity.IncrementVersion();
			mFreeEntities.pop_back();
			mEntityPool[newEntity.GetId()] = newEntity;
		}
		return newEntity;
	}

	void EntityManager::RemoveEntity(Entity entity)
	{
		ZE_ASSERT(std::all_of(mComponentPoolMap.cbegin(), mComponentPoolMap.cend(), 
			[&entity](auto&& container) { return container.second->Has(entity.GetId()); }), "Expected entity components to be removed");
		if (entity.GetId() < mEntityPool.size()) {
			Entity& foundEntity = mEntityPool[entity.GetId()];
			if(!foundEntity.isValid() || entity != foundEntity) {
				return;
			}
			foundEntity = Entity(Entity::INVALID_ID, Entity::INVALID_VERSION);
			mFreeEntities.push_back(entity);
		}
	}
	
}
