// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Render/Mesh/MeshAttribute.h>

namespace Zeron::Render
{
	class MeshInfo {
	  public:
		template <MeshAttribute E, typename DataType>
		void AddAttributeData(const DataType* data, size_t size)
		{
			using AttrType = typename MeshAttributeTypeInfo<E>::DataType;
			static_assert(sizeof(DataType) % sizeof(AttrType) == 0);
			constexpr size_t packCount = sizeof(DataType) / sizeof(AttrType);
			const size_t packSize = size * packCount;
			auto& attr = GetAttribute<E>();

			if (attr.GetStride() == 0) {
				attr = MeshAttributeData<AttrType>(
					std::vector<AttrType>(reinterpret_cast<const AttrType*>(data), reinterpret_cast<const AttrType*>(data) + packSize), packCount
				);
			}
			else {
				ZE_ASSERT(attr.GetStride() == packCount * sizeof(AttrType), "Expected correct stride for mesh attribute");
				attr.Append(reinterpret_cast<const AttrType*>(data), packSize);
			}
		}

		template <MeshAttribute E>
		void ClearAttributeData()
		{
			GetAttribute<E>().Clear();
		}

		template <MeshAttribute E>
		bool HasAttribute() const
		{
			return GetAttribute<E>().GetStride() != 0;
		}

		template <MeshAttribute E>
		MeshAttributeData<typename MeshAttributeTypeInfo<E>::DataType>& GetAttribute()
		{
			return std::get<Attr<E>>(mAttributes).mAttribute;
		}

	  private:
		template <MeshAttribute E>
		struct Attr {
			MeshAttributeData<typename MeshAttributeTypeInfo<E>::DataType> mAttribute;
		};

		std::tuple<Attr<MeshAttribute::Position>, Attr<MeshAttribute::Normal>, Attr<MeshAttribute::UV>, Attr<MeshAttribute::Color>, Attr<MeshAttribute::Index>> mAttributes;
	};


}
