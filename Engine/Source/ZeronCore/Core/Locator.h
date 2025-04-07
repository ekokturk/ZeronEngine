// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	class Locator {
	  public:
		template <typename T>
		static void Register(T* service)
		{
			services[typeid(T)] = reinterpret_cast<void*>(service);
		}

		template <typename T>
		static T* Get()
		{
			auto it = services.find(typeid(T));
			if (it != services.end()) {
				return static_cast<T*>(it->second);
			}
			ZE_FAIL("Expected service to exist");
			return nullptr;
		}

	  private:
		static inline std::unordered_map<std::type_index, void*> services;
	};
}