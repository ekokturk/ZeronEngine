// Copyright (C) Eser Kokturk. All Rights Reserved.

#include "Mesh.h"

#include "Graphics/Buffer.h"
#include "Graphics/Graphics.h"
#include "Graphics/GraphicsContext.h" 
#include "Graphics/Texture.h"

namespace Zeron
{
	Mesh::Mesh(Graphics& graphics, const std::vector<Vertex>& vertices, 
		const std::vector<unsigned long>& indices, 
		const Mat4& transform)
	{
		mVertexBuffer = graphics.CreateVertexBuffer<Vertex>(vertices);
		mIndexBuffer = graphics.CreateIndexBuffer(indices);
		mTransform = transform;
	}

	void Mesh::Draw(GraphicsContext& ctx)
	{
		ctx.SetVertexBuffer(*mVertexBuffer);
		ctx.SetIndexBuffer(*mIndexBuffer);
		ctx.DrawIndexed(mIndexBuffer->GetCount(), 0);
	}

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
