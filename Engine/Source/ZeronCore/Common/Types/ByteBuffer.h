// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	class ByteBuffer : public std::vector<std::byte> {
		using BaseType = std::vector<std::byte>;
		using BaseType::BaseType;

	  public:
		template <typename T>
		T* GetAs()
		{
			return reinterpret_cast<T*>(data());
		}

		template <typename T>
		T* GetAs() const
		{
			return reinterpret_cast<T*>(data());
		}
	};
}