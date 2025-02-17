// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Physics/CollisionShape.h>
#include <Physics/World.h>

namespace physx
{
	class PxDefaultCpuDispatcher;
	class PxScene;
}

namespace Zeron::Physics
{
	class LibraryPhysX;

	class WorldPhysX : public World {
	  public:
		WorldPhysX(LibraryPhysX& library, const WorldSettings& settings);
		~WorldPhysX();


		void Simulate(float elapsedTime) override;
		void AddBody(CollisionBody& body) override;
		void RemoveBody(CollisionBody& body) override;
		std::vector<CollisionBody*> Overlap(const CollisionShape& shape, const Mat4& transform) override;
		std::vector<CollisionBody*> RayCast(const Vec3& origin, const Vec3& direction, float distance) override;

	  private:
		physx::PxScene* mScene;
		physx::PxDefaultCpuDispatcher* mDispatcher;
	};
}