// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Entity/ComponentContainer.h>
#include <Entity/Entity.h>

namespace Zeron
{
	class EntityManager {
	  public:
		Entity CreateEntity();
		void RemoveEntity(Entity entity);

		Entity GetEntity(Entity::Id id) const { return mEntityPool[id]; }

		bool HasEntity(Entity entity) const { return entity.GetId() < mEntityPool.size() && mEntityPool[entity.GetId()] == entity; }

		template <typename ComponentType>
		ComponentType& GetComponent(Entity entity)
		{
			ZE_ASSERT(HasEntity(entity), "Entity was not found");
			return _getContainerAs<ComponentType>().GetComponent(entity.GetId());
		}

		template <typename ComponentType>
		const ComponentType& GetComponent(Entity entity) const
		{
			ZE_ASSERT(HasEntity(entity), "Entity was not found");
			return _getContainerAs<ComponentType>().GetComponent(entity.GetId());
		}

		template <typename ComponentType>
		ComponentType* TryGetComponent(Entity entity)
		{
			return _hasContainer<ComponentType>() && HasEntity(entity) ? _getContainerAs<ComponentType>().TryGetComponent(entity.GetId()) : nullptr;
		}

		template <typename ComponentType>
		bool HasComponent(Entity entity) const
		{
			return _hasContainer<ComponentType>() && HasEntity(entity) ? _getContainerAs<ComponentType>().Has(entity.GetId()) : false;
		}

		template <typename ComponentType, typename... Args>
		void AddComponent(Entity entity, Args... args)
		{
			auto& container = !_hasContainer<ComponentType>() ? _createContainer<ComponentType>() : _getContainerAs<ComponentType>();
			container.AddComponent(entity.GetId(), std::forward<Args>(args)...);
		}

		template <typename ComponentType>
		void RemoveComponent(Entity entity)
		{
			if (_hasContainer<ComponentType>()) {
				_getContainerAs<ComponentType>().RemoveComponent(entity.GetId());
			}
		}

		template <typename... ComponentTypes>
		class View {
			using IterableContainerType = ComponentContainer<typename TypeTraits::FirstType<ComponentTypes...>::type>;
			using IterableContainerIterator = typename IterableContainerType::Iterator;
			using ComponentPackRefType = std::tuple<ComponentTypes&...>;
			using ComponentPackRefWithEntityType = std::tuple<Entity, ComponentTypes&...>;
			using ContainerPackRefType = std::tuple<ComponentContainer<ComponentTypes>&...>;

		  public:
			class Iterator {
			  public:
				Iterator(View* view, IterableContainerIterator itr)
					: mView(view)
					, mItr(itr)
				{}

				Iterator& operator++()
				{
					do {
						++mItr;
					}
					while (mView->_getIterableContainer().end() != mItr &&
						   !std::apply(
							   [&](auto&... container) {
								   return (container.Has(mItr->second) && ...);
							   },
							   mView->_getAdditionalContainers()
						   ));
					return *this;
				}

				bool operator==(const Iterator& other) const { return mItr == other.mItr; }

				bool operator!=(const Iterator& other) const { return mItr != other.mItr; }

				ComponentPackRefWithEntityType operator*()
				{
					return std::apply(
						[&](auto&... container) {
							return std::make_tuple(mView->mManager.GetEntity(mItr->second), std::ref(mItr->first), std::ref(container.GetComponent(mItr->second))...);
						},
						mView->_getAdditionalContainers()
					);
				}

			  private:
				View* mView = nullptr;
				IterableContainerIterator mItr;
			};

			template <size_t comps = sizeof...(ComponentTypes), std::enable_if_t<comps != 0, int> = 0>
			View(EntityManager& manager)
				: mManager(manager)
				, mAllContainers(std::make_tuple(std::ref(mManager._getContainerAs<ComponentTypes>())...))
			{}

			Iterator begin()
			{
				for (auto it = _getIterableContainer().begin(); it != _getIterableContainer().end(); ++it) {
					if (_verifyAdditionalContainers(it->second)) {
						return Iterator(this, it);
					}
				}
				return end();
			}

			Iterator end() { return Iterator(this, _getIterableContainer().end()); }

			template <typename Callback>
			void ForEach(Callback&& callback)
			{
				for (auto&& [component, entityId] : _getIterableContainer()) {
					const Entity& entity = mManager.GetEntity(entityId);

					// TODO: Check for excluded components

					// Check if entity has additional components
					if (!_verifyAdditionalContainers(entityId)) {
						continue;
					}

					if constexpr (std::is_invocable_v<decltype(callback), Entity>) {
						callback(entity);
					}
					else {
						auto additionalArguments = std::apply(
							[this, id = entityId, comp = std::ref(component)](auto&... container) {
								return std::make_tuple(comp, std::ref(container.GetComponent(id))...);
							},
							_getAdditionalContainers()
						);
						if constexpr (std::is_invocable_v<decltype(callback), ComponentTypes&...>) {
							std::apply(callback, additionalArguments);
						}
						else {
							std::apply(callback, std::tuple_cat(std::make_tuple(entity), additionalArguments));
						}
					}
				}
			}

		  private:
			IterableContainerType& _getIterableContainer() { return std::get<0>(mAllContainers); }

			auto _getAdditionalContainers()
			{
				return std::apply(
					[](auto& first, auto&... containers) {
						return std::make_tuple(std::ref(containers)...);
					},
					mAllContainers
				);
			}

			bool _verifyAdditionalContainers(Entity::Id id)
			{
				return std::apply(
					[id](auto&... container) {
						return (container.Has(id) && ...);
					},
					_getAdditionalContainers()
				);
			}

			EntityManager& mManager;
			ContainerPackRefType mAllContainers;
		};

		template <typename... ComponentTypes>
		View<ComponentTypes...> CreateView()
		{
			return View<ComponentTypes...>(*this);
		}

		template <typename... ComponentTypes, typename Callback>
		void ForEach(Callback&& callback)
		{
			// Iterate over all valid entities
			if constexpr (sizeof...(ComponentTypes) == 0) {
				for (Entity entity : mEntityPool) {
					if (entity.isValid()) {
						callback(entity);
					}
				}
			}
			// Iterate over all entities with specified components
			else {
				CreateView<ComponentTypes...>().ForEach(std::forward<Callback>(callback));
			}
		}

	  private:
		// TODO: Maybe create static component Id from an integer counter
		template <typename ComponentType>
		constexpr size_t _getComponentId() const
		{
			return typeid(ComponentType).hash_code();
		}

		template <typename ComponentType>
		ComponentContainer<ComponentType>& _createContainer()
		{
			return static_cast<ComponentContainer<ComponentType>&>(
				*mComponentPoolMap.emplace(_getComponentId<ComponentType>(), std::make_unique<ComponentContainer<ComponentType>>()).first->second
			);
		}

		template <typename ComponentType>
		ComponentContainer<ComponentType>& _getContainerAs() const
		{
			return static_cast<ComponentContainer<ComponentType>&>(*mComponentPoolMap.at(_getComponentId<ComponentType>()));
		}

		template <typename ComponentType>
		ComponentContainer<ComponentType>& _getContainerAs()
		{
			return static_cast<ComponentContainer<ComponentType>&>(*mComponentPoolMap.at(_getComponentId<ComponentType>()));
		}

		template <typename ComponentType>
		bool _hasContainer() const
		{
			return mComponentPoolMap.count(_getComponentId<ComponentType>()) > 0;
		}

		std::vector<Entity> mEntityPool;
		// TODO: Change this to a list
		std::vector<Entity> mFreeEntities;
		std::unordered_map<size_t, std::unique_ptr<IComponentContainer>> mComponentPoolMap;
	};
}