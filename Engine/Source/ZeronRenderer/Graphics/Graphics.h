// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
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

	};
}
