// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Render/Mesh/MeshAttribute.h>

namespace Zeron::Render
{
	class MeshDescription {
	  public:
		MeshDescription(size_t id = 0, std::string name = "")
			: mId(id)
			, mName(std::move(name))
		{}

	  private:
		size_t mId;
		std::string mName;
	};

	class MeshData {
	  public:
		using Index = uint32_t;
		using IndexList = std::vector<Index>;
		using VertexMap = std::unordered_map<MeshAttribute, ByteBuffer>;

		MeshData(MeshDescription desc = {})
			: mDesc(std::make_shared<MeshDescription>(std::move(desc)))
		{}

		MeshData(VertexMap vertices, IndexList indices, MeshDescription desc = {})
			: mVertexData(std::move(vertices))
			, mIndices(std::move(indices))
			, mDesc(std::make_shared<MeshDescription>(std::move(desc)))
		{}

		template <MeshAttribute T>
		void AppendVertex(const typename MeshAttributeInfo<T>::Type& data)
		{
			mVertexData[T].Append<typename MeshAttributeInfo<T>::Type>(data);
		}

		template <MeshAttribute T>
		void AppendVertices(std::span<const typename MeshAttributeInfo<T>::Type> data)
		{
			mVertexData[T].Append<typename MeshAttributeInfo<T>::Type>(data);
		}

		void AppendIndex(Index data) { mIndices.push_back(data); }

		void AppendIndices(std::span<const Index> data) { mIndices.insert(mIndices.end(), data.begin(), data.end()); }

		template <MeshAttribute T>
		std::span<const typename MeshAttributeInfo<T>::Type> GetVertices() const
		{
			using AttrType = const typename MeshAttributeInfo<T>::Type;

			auto itr = mVertexData.find(T);
			if (itr != mVertexData.end()) {
				return std::span<AttrType>{ reinterpret_cast<AttrType*>(itr->second.data()), itr->second.size() / sizeof(AttrType) };
			}

			return std::span<AttrType>();
		}

		void ForEachAttribute(const std::function<void(MeshAttribute, const ByteBuffer&, size_t count, size_t stride)>& callback) const
		{
			for (auto&& data : mVertexData) {
				if (!data.second.empty()) {
					const size_t stride = GetMeshAttributeSize(data.first);
					callback(data.first, data.second, data.second.size() / stride, stride);
				}
			}
		}

		const IndexList& GetIndices() const { return mIndices; }

		const std::shared_ptr<const MeshDescription>& GetDescription() const { return mDesc; }

		size_t GetIndexCount() const { return mIndices.size(); }

	  private:
		std::unordered_map<MeshAttribute, ByteBuffer> mVertexData;
		IndexList mIndices;
		std::shared_ptr<const MeshDescription> mDesc;
	};
}