﻿// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/Primitives.h>

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
			const Mat4& transform);
		Mesh(const Mesh& mesh) = default;

		void Draw(GraphicsContext& ctx);

		const Mat4& GetTransform() const;
		Buffer* GetVertexBuffer() const;
		Buffer* GetIndexBuffer() const;
	
	private:
		std::shared_ptr<Buffer> mVertexBuffer;
		std::shared_ptr<Buffer> mIndexBuffer;
		Mat4 mTransform;
	};
}

