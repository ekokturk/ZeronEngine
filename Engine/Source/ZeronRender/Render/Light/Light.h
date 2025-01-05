// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	namespace Render
	{
		struct alignas(16) Light {
			enum Type
			{
				Invalid = 0,
				Directional = 1,
				Point = 2,
				Spot = 3
			};

			Vec4 mColor = Vec4(1.f);
			Vec4 mPosition;
			Vec4 mDirection;
			Vec4 mViewPosition;
			Vec4 mViewDirection;
			Vec2 mSpotAngle;
			float mRange = 0.f;
			Type mType = Type::Invalid;
		};
	}
}
