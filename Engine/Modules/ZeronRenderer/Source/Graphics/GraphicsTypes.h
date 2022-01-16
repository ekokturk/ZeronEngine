// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	enum class GraphicsType {
		Null,
		OpenGLES,
		Direct3D11,
	};

	enum class PrimitiveTopology
	{
		Invalid = 0,
		PointList,
		TriangleList,
	};
	
	enum class BufferType {
		Vertex,
		Index,
		Constant
	};

	enum class ShaderType {
		Vertex,
		Fragment,
		Invalid,
	};

	enum class TextureType {
		Invalid,
		Diffuse,
		Normal,
	};
}
