// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "Primitives.h"

namespace Zeron {
	class Window;

	enum class GraphicsType {
		Null,
		OpenGLES,
		Direct3D11,
	};
	
	class Graphics
	{
	public:
		virtual ~Graphics() = default;
		virtual bool Init() = 0;
		virtual bool Destroy() = 0;
		virtual void RenderFrame() = 0;
		virtual void Draw() = 0;

		virtual void CreateGraphicsContext(Window* window) = 0;
		virtual void CreateVertexBuffer(const std::vector<Vertex>& data) = 0;
	};
}
