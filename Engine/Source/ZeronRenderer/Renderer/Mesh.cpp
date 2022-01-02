// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Mesh.h"

#include "Graphics/Buffer.h"
#include "Graphics/Graphics.h"
#include "Graphics/GraphicsContext.h" 
#include "Graphics/Texture.h"

namespace Zeron
{
	Mesh::Mesh(Graphics& graphics, const std::vector<Vertex>& vertices, 
		const std::vector<unsigned long>& indices, 
		const std::vector<std::shared_ptr<Texture>>& textures,
		const Mat4& transform)
	{
		mVertexBuffer = graphics.CreateVertexBuffer<Vertex>(vertices);
		mIndexBuffer = graphics.CreateIndexBuffer(indices);
		mTextures = textures;
		mTransform = transform;
	}

	void Mesh::Draw(GraphicsContext& ctx)
	{
		for(auto& texture : mTextures) {
			ctx.SetTexture(texture.get());
		}
		
		ctx.SetVertexBuffer(*mVertexBuffer);
		ctx.SetIndexBuffer(*mIndexBuffer);
		ctx.DrawIndexed(mIndexBuffer->GetCount(), 0);
	}

	void Mesh::AddTexture(const std::shared_ptr<Texture>& texture)
	{
		ZE_ASSERT(texture, "Texture cannot be null!");
		mTextures.push_back(texture);
	}

	void Mesh::SetTextures(const std::vector<std::shared_ptr<Texture>>& textures)
	{
		mTextures = textures;
	}

	const Mat4& Mesh::GetTransform() const
	{
		return mTransform;
	}
}
