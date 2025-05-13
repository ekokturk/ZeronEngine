// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	class ByteBuffer : public std::vector<std::byte> {
		using BaseType = std::vector<std::byte>;
		using BaseType::BaseType;

	  public:
		using View = std::span<const std::byte>;

		template <typename T>
		static ByteBuffer Create(std::span<T> data)
		{
			ByteBuffer byteBuffer(data.size_bytes());
			std::memcpy(byteBuffer.data(), data.data(), data.size_bytes());
			return byteBuffer;
		}

		ByteBuffer Copy() const { return { begin(), end() }; }

		ByteBuffer::View GetView(size_t offset = 0) const
		{
			if (offset == 0 || offset >= size()) {
				return View(*this);
			}
			return View(*this).subspan(offset);
		}

		template <typename T>
		bool CopyTo(T& obj) const
		{
			if constexpr (TypeTraits::ContainerType<T>) {
				if (size() % sizeof(typename T::value_type) == 0) {
					obj.resize(size() / sizeof(typename T::value_type));
					std::memcpy(obj.data(), data(), size());
					return true;
				}
			}
			else {
				if (size() == sizeof(T)) {
					std::memcpy(&obj, data(), sizeof(T));
					return true;
				}
			}
			return false;
		}

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
		bool IsEqual(const std::span<const T>& other) const
		{
			auto view = GetView();
			auto otherView = std::span<const std::byte>(reinterpret_cast<const std::byte*>(other.data()), other.size_bytes());
			return view.size() == otherView.size() && std::equal(view.begin(), view.end(), otherView.begin());
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