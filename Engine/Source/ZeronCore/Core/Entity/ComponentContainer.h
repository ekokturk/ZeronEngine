// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Core/Entity/Entity.h>
#include <Core/Types/SparseSet.h>

namespace Zeron
{
	class IComponentContainer {
	public:
		virtual ~IComponentContainer() = default;

		virtual bool Has(Entity::Id entity) const = 0;
		virtual const std::type_info& GetType() const = 0;
	};

	template<typename Type>
	class ComponentContainer final : public IComponentContainer {
	public:
		using TypeIndexPair = std::pair<Type, Entity::Id>;
		using ContainerType = SparseSet<TypeIndexPair, Entity::Id, Entity::INVALID_ID>;
		using Iterator = typename ContainerType::Iterator;

		~ComponentContainer() {}

		Type& GetComponent(Entity::Id entityId)
		{
			return mContainer.Get(entityId).first;
		}

		Type& GetComponent(Entity::Id entityId) const
		{
			return mContainer.Get(entityId).first;
		}

		Type* TryGetComponent(Entity::Id entityId)
		{
			return &mContainer.TryGet(entityId)->first;
		}

		bool Has(Entity::Id entityId) const override
		{
			return mContainer.Contains(entityId);
		}

		template<typename ...Args>
		void AddComponent(Entity::Id entityId, Args&&... args)
		{
			mContainer.Add(entityId, std::make_pair(Type(std::forward<Args>(args)...), entityId));
		}

		void RemoveComponent(Entity::Id entityId)
		{
			if(mContainer.Contains(entityId)) {
				mContainer.Remove(entityId);
			}
		}

		const std::type_info& GetType() const override {
			return typeid(Type);
		}

		size_t GetSize() const
		{
			return mContainer.GetSize();
		}

		std::vector<TypeIndexPair>& GetComponentList()
		{
			return mContainer.GetData();
		}

		Iterator begin()
		{
			return mContainer.begin();
		}

		Iterator end()
		{
			return mContainer.end();
		}
		
	private:
		ContainerType mContainer;
	};
}
