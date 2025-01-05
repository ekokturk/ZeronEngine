// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron::Gfx
{
	struct Vertex {
		Vec3 mPosition;
		Vec2 mTextureCoord;
		Vec3 mNormal;
		Vec4 mTangent;
	};

	struct VertexInstance {
		Vec3 mInstancePosition;
	};
}