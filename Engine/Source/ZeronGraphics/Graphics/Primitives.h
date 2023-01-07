// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	struct Vertex
	{
		 Vec3 mPosition;
		 Vec2 mTextureCoord;
		 Vec3 mNormal;
	};

	struct VertexInstance {
		Vec3 mInstancePosition;
	};
}
