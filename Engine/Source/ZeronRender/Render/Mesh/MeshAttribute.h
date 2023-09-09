// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron::Render
{
	enum class MeshAttribute
	{
		Index,
		Position,
		Normal,
		UV,
		Color,
	};

	template <typename T>
	struct MeshAttributeData {
		MeshAttributeData() = default;

		MeshAttributeData(std::vector<T>&& data, size_t packCount)
			: mData(std::move(data))
			, mPackCount(packCount)
		{}

		void Append(const T* data, size_t size)
		{
			mData.reserve(mData.size() + size);
			std::copy(data, data + size, std::back_inserter(mData));
		}

		void Clear()
		{
			mData.clear();
			mPackCount = 0;
		}

		size_t GetStride() const { return mPackCount * sizeof(T); }
		size_t GetSize() const { return mPackCount == 0 ? 0 : mData.size() / mPackCount; }
		const T* GetData() const { return mData.data(); }

	  private:
		std::vector<T> mData;
		size_t mPackCount = 0;
	};

	template <MeshAttribute E>
	struct MeshAttributeTypeInfo {};

	template <>
	struct MeshAttributeTypeInfo<MeshAttribute::Position> {
		using DataType = float;
	};

	template <>
	struct MeshAttributeTypeInfo<MeshAttribute::Normal> {
		using DataType = float;
	};

	template <>
	struct MeshAttributeTypeInfo<MeshAttribute::UV> {
		using DataType = float;
	};

	template <>
	struct MeshAttributeTypeInfo<MeshAttribute::Color> {
		using DataType = float;
	};

	template <>
	struct MeshAttributeTypeInfo<MeshAttribute::Index> {
		using DataType = uint32_t;
	};
}
