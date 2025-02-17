// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Physics/CollisionBody.h>

namespace physx
{
	class PxRigidStatic;
	class PxActor;
}

namespace Zeron::Physics
{
	class MaterialPhysX;
	class LibraryPhysX;

	class CollisionBodyPhysX : public CollisionBody {
	  public:
		CollisionBodyPhysX(LibraryPhysX& library, const CollisionBodyConfig& config);
		~CollisionBodyPhysX() override;

		Mat4 GetTransform() const override;
		Vec3 GetPosition() const override;

		physx::PxRigidStatic& GetRigidPX();

	  private:
		MaterialPhysX* mMaterial = nullptr;
		physx::PxRigidStatic* mBody = nullptr;
		Mat4 mTransform;
	};
}