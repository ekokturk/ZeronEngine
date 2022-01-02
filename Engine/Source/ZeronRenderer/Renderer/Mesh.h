// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "Graphics/Primitives.h"


namespace Zeron
{
	class Texture;
	class GraphicsContext;
	class Graphics;
	class Buffer;

	class Mesh {
	public:
		Mesh(Graphics& graphics, const std::vector<Vertex>& vertices, 
			const std::vector<unsigned long>& indices, 
			const std::vector<std::shared_ptr<Texture>>& textures,
			const Mat4& transform);
		Mesh(const Mesh& mesh) = default;

		void Draw(GraphicsContext& ctx);

		void AddTexture(const std::shared_ptr<Texture>& texture);
		void SetTextures(const std::vector<std::shared_ptr<Texture>>& textures);
		
		[[nodiscard]] const Mat4& GetTransform() const;
	
	private:
		std::shared_ptr<Buffer> mVertexBuffer;
		std::shared_ptr<Buffer> mIndexBuffer;
		std::vector<std::shared_ptr<Texture>> mTextures;
		Mat4 mTransform;
	};
}


