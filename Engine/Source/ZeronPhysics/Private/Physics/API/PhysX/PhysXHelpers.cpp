// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Physics/API/PhysX/PhysXHelpers.h>

#include <Physics/API/PhysX/CollisionBodyPhysX.h>
#include <Physics/API/PhysX/DynamicBodyPhysX.h>
#include <Physics/CollisionType.h>
#include <PxRigidDynamic.h>
#include <PxRigidStatic.h>

namespace Zeron::Physics::Px
{
	physx::PxActor* GetActor(CollisionBody& body)
	{
		switch (body.GetCollisionType()) {
			case CollisionType::Static: return &static_cast<CollisionBodyPhysX&>(body).GetRigidPX();
			case CollisionType::Dynamic: return &static_cast<DynamicBodyPhysX&>(body).GetRigidPX();
			default: ZE_FAIL("Invalid collision body");
		}
		return nullptr;
	}

	physx::PxTransform ToTransform(const Mat4& transform)
	{
		const Quat quat(transform);
		return { physx::PxVec3{ transform[3].x, transform[3].y, transform[3].z }, physx::PxQuat{ quat.x, quat.y, quat.z, quat.w } };
	}

	Mat4 FromTransform(const physx::PxTransform& transform)
	{
		const Vec3 translation(transform.p.x, transform.p.y, transform.p.z);
		const Quat rotation(transform.q.w, transform.q.x, transform.q.y, transform.q.z);
		return Math::MakeTransform(translation, rotation);
	}

	std::unique_ptr<physx::PxGeometry> GetGeometry(const CollisionShape& shape)
	{
		return std::visit(
			TypeTraits::Visitor{
				[](const BoxShape& s) -> std::unique_ptr<physx::PxGeometry> {
					return std::make_unique<physx::PxBoxGeometry>(s.mHalfExtents.X, s.mHalfExtents.Y, s.mHalfExtents.Z);
				},
				[](const SphereShape& s) -> std::unique_ptr<physx::PxGeometry> {
					return std::make_unique<physx::PxSphereGeometry>(s.mRadius);
				},
				[](const CapsuleShape& s) -> std::unique_ptr<physx::PxGeometry> {
					return std::make_unique<physx::PxCapsuleGeometry>(s.mRadius, s.mHalfHeight);
				},
				[](const PlaneShape& s) -> std::unique_ptr<physx::PxGeometry> {
					return std::make_unique<physx::PxPlaneGeometry>();
				},
			},
			shape
		);
	}
}