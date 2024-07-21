// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Script/ScriptObjectStorage.h>
#include <Script/TypeInfo.h>

namespace Zeron::Script
{
	class ObjectManager {
	  public:
		virtual ~ObjectManager() = default;

		template <typename Class, typename... Args>
		ObjectInstance CreateInstance(Args... args)
		{
			if (ObjectStorage* storage = GetStorage<Class>()) {
				ObjectInstance::Owner owningObj = nullptr;
				if constexpr (sizeof...(Args) > 0) {
					owningObj = std::static_pointer_cast<void>(std::make_shared<Class>(std::forward<Args>(args)...));
				}
				else {
					owningObj = std::static_pointer_cast<void>(std::make_shared<Class>());
				}
				auto handle = storage->GetOrAddInstance(owningObj.get(), owningObj);
				return { handle, storage->GetTypeInfo() };
			}
			return NullObject;
		}

		template <typename Class>
		ObjectInstance TrackInstance(Class* nativeObj)
		{
			if (ObjectStorage* storage = GetStorage<Class>()) {
				auto handle = storage->GetOrAddInstance(nativeObj);
				return { handle, storage->GetTypeInfo() };
			}
			return NullObject;
		}

		template <typename Class>
		ObjectStorage* GetStorage()
		{
			return GetStorage(TypeInfo::GetId<Class>());
		}

		ObjectStorage* GetStorage(size_t id)
		{
			auto storage = mStorage.find(id);
			if (storage != mStorage.end()) {
				return storage->second.get();
			}
			return nullptr;
		}

	  protected:
		std::unordered_map<uint64_t, std::unique_ptr<ObjectStorage>> mStorage;
	};
}