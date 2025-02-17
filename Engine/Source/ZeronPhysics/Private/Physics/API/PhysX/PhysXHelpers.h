// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <foundation/PxTransform.h>
#include <foundation/PxVec3.h>
#include <geometry/PxCapsuleGeometry.h>
#include <Physics/CollisionShape.h>

namespace physx
{
	class PxActor;
	class PxGeometry;
}
namespace Zeron::Physics
{
	class CollisionBody;

	namespace Px
	{
		inline physx::PxVec3 ToVec3(const Vec3& vec3)
		{
			return physx::PxVec3{ vec3.X, vec3.Y, vec3.Z };
		}

		inline Vec3 FromVec3(const physx::PxVec3& vec3)
		{
			return Vec3{ vec3.x, vec3.y, vec3.z };
		}

		physx::PxTransform ToTransform(const Mat4& transform);

		Mat4 FromTransform(const physx::PxTransform& transform);

		physx::PxActor* GetActor(CollisionBody& body);

		std::unique_ptr<physx::PxGeometry> GetGeometry(const CollisionShape& shape);
	}
}