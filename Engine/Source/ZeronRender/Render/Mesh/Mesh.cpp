// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Render/Mesh/Mesh.h>

#include <Graphics/Buffer.h>
#include <Graphics/CommandBuffer.h>
#include <Graphics/Graphics.h>

namespace Zeron::Render
{
	Mesh::Mesh(Gfx::Graphics& graphics, const MeshData& data)
		: mDescription(data.GetDescription())
	{
		data.ForEachAttribute([&](MeshAttribute attr, const ByteBuffer& buff, size_t count, size_t stride) {
			mVertexBuffers[attr] = graphics.CreateBuffer(Gfx::BufferType::Vertex, count, stride, buff.data());
		});
		if (!data.GetIndices().empty()) {
			mIndexBuffer = graphics.CreateIndexBuffer(data.GetIndices());
		}
	}

	Mesh::~Mesh() {}

	const MeshDescription& Mesh::GetMeshDescription() const
	{
		return *mDescription;
	}

	uint32_t Mesh::GetIndexCount() const
	{
		return mIndexBuffer->GetCount();
	}

	void Mesh::Bind(Gfx::CommandBuffer& cmd, std::span<const MeshAttribute> attributes) const
	{
		std::vector<Gfx::Buffer*> buffers;
		for (MeshAttribute attr : attributes) {
			auto itr = mVertexBuffers.find(attr);
			if (itr == mVertexBuffers.end()) {
				ZE_FAIL("Mesh: Requested invalid vertex buffer for mesh attribute.");
			}
			buffers.push_back(itr->second.get());
		}

		cmd.SetVertexBuffers(buffers.data(), buffers.size());
		if (mIndexBuffer) {
			cmd.SetIndexBuffer(*mIndexBuffer);
		}
	}
}