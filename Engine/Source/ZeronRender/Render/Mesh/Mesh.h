﻿// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/Primitives.h>

namespace Zeron
{
	namespace Gfx
	{
		class GraphicsContext;
		class Graphics;
		class Buffer;
	}

	class Texture;

	class Mesh {
	  public:
		Mesh(Gfx::Graphics& graphics, const std::vector<Gfx::Vertex>& vertices, const std::vector<uint32_t>& indices);
		Mesh(const Mesh& mesh) = default;
		~Mesh();

		Gfx::Buffer* GetVertexBuffer() const;
		Gfx::Buffer* GetIndexBuffer() const;

	  private:
		std::shared_ptr<Gfx::Buffer> mVertexBuffer;
		std::shared_ptr<Gfx::Buffer> mIndexBuffer;
	};
}