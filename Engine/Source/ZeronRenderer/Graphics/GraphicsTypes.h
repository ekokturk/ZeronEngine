// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

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
	

}
