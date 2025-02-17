// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Physics/API/PhysX/CollisionBodyPhysX.h>

#include <extensions/PxSimpleFactory.h>
#include <Physics/API/PhysX/LibraryPhysX.h>
#include <Physics/API/PhysX/MaterialPhysX.h>
#include <Physics/API/PhysX/PhysXHelpers.h>
#include <PxRigidStatic.h>

namespace Zeron::Physics
{
	CollisionBodyPhysX::CollisionBodyPhysX(LibraryPhysX& library, const CollisionBodyConfig& config)
	{
		mId = config.mId;
		physx::PxMaterial& mat = mMaterial ? mMaterial->GetMaterialPX() : library.GetDefaultMaterial();
		const physx::PxTransform transform = Px::ToTransform(config.mTransform);
		const std::unique_ptr<physx::PxGeometry> geo = Px::GetGeometry(config.mShape);
		mBody = PxCreateStatic(library.GetPhysicsPX(), transform, *geo, mat);
		ZE_ASSERT(mBody, "RigidBodyPhysX: Could not create rigid body.");
		mTransform = Px::FromTransform(mBody->getGlobalPose());
		mBody->userData = this;
	}

	CollisionBodyPhysX::~CollisionBodyPhysX()
	{
		mBody->release();
	}

	Mat4 CollisionBodyPhysX::GetTransform() const
	{
		return mTransform;
	}

	Vec3 CollisionBodyPhysX::GetPosition() const
	{
		return Math::GetPos(mTransform);
	}

	physx::PxRigidStatic& CollisionBodyPhysX::GetRigidPX()
	{
		return *mBody;
	}
}