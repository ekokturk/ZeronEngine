// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron::Physics
{
	struct BoxShape {
		Vec3 mHalfExtents{ 0.5f };
	};

	struct SphereShape {
		float mRadius = 0.5f;
	};

	struct CapsuleShape {
		float mRadius = 0.5f;
		float mHalfHeight = 0.5f;
	};

	struct PlaneShape {
		Vec3 mNormal;
		float mLength;
	};

	using CollisionShape = std::variant<BoxShape, SphereShape, CapsuleShape, PlaneShape>;
}