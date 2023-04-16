// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Renderer/Scene/Mesh.h>

#include <Graphics/Buffer.h>
#include <Graphics/Graphics.h>
#include <Graphics/GraphicsContext.h>

namespace Zeron
{
	Mesh::Mesh(Graphics& graphics, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Mat4& transform)
	{
		mVertexBuffer = graphics.CreateVertexBuffer<Vertex>(vertices);
		mIndexBuffer = graphics.CreateIndexBuffer(indices);
		mTransform = transform;
	}

	Mesh::~Mesh() {}

	const Mat4& Mesh::GetTransform() const
	{
		return mTransform;
	}

	Buffer* Mesh::GetVertexBuffer() const
	{
		return mVertexBuffer.get();
	}

	Buffer* Mesh::GetIndexBuffer() const
	{
		return mIndexBuffer.get();
	}
}