// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	class ByteBuffer : public std::vector<std::byte> {
		using BaseType = std::vector<std::byte>;
		using BaseType::BaseType;

	  public:
		template <typename T>
		static ByteBuffer Create(std::span<T> data)
		{
			ByteBuffer byteBuffer(data.size_bytes());
			std::memcpy(byteBuffer.data(), data.data(), data.size_bytes());
			return byteBuffer;
		}

		ByteBuffer Copy() const { return { begin(), end() }; }

		template <typename T>
		ByteBuffer& Append(const T& val)
		{
			const size_t dataSize = sizeof(T);
			const size_t currentSize = size();
			resize(currentSize + dataSize);
			std::memcpy(&front() + currentSize, reinterpret_cast<const std::byte*>(&val), dataSize);
			return *this;
		}

		template <typename T>
		ByteBuffer& Append(std::span<const T> val)
		{
			const size_t dataSize = val.size_bytes();
			const size_t currentSize = size();
			resize(currentSize + dataSize);
			std::memcpy(&front() + currentSize, reinterpret_cast<const std::byte*>(val.data()), dataSize);
			return *this;
		}

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