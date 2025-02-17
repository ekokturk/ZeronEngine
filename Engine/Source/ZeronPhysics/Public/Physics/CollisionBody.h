// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Physics/CollisionShape.h>
#include <Physics/CollisionType.h>

namespace Zeron::Physics
{
	class Material;

	struct CollisionBodyConfig {
		int mId = -1;
		Mat4 mTransform;
		Material* mMaterial = nullptr;
		CollisionShape mShape = SphereShape{};
	};

	class CollisionBody {
	  public:
		virtual ~CollisionBody() = default;
		virtual CollisionType GetCollisionType() const { return CollisionType::Static; }
		virtual Mat4 GetTransform() const = 0;
		virtual Vec3 GetPosition() const = 0;

		int GetId() const { return mId; }

	  protected:
		int mId = -1;
	};
}