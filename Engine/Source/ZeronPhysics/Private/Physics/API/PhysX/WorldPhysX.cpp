// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Physics/API/Physx/WorldPhysx.h>

#include <extensions/PxDefaultCpuDispatcher.h>
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <Physics/API/PhysX/DynamicBodyPhysX.h>
#include <Physics/API/PhysX/LibraryPhysX.h>
#include <Physics/API/PhysX/PhysXHelpers.h>
#include <PxPhysics.h>
#include <PxRigidActor.h>
#include <PxScene.h>

namespace Zeron::Physics
{

	WorldPhysX::WorldPhysX(LibraryPhysX& library, const WorldSettings& settings)
		: World(settings)
	{
		mDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		physx::PxSceneDesc sceneDesc(library.GetPhysicsPX().getTolerancesScale());
		sceneDesc.gravity = Px::ToVec3(settings.mGravity);
		sceneDesc.cpuDispatcher = mDispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		mScene = library.GetPhysicsPX().createScene(sceneDesc);
		ZE_ASSERT(mScene, "WorldPhysX: Could not create scene.");
	}

	WorldPhysX::~WorldPhysX()
	{
		mScene->release();
		mDispatcher->release();
	}

	void WorldPhysX::Simulate(float elapsedTime)
	{
		mScene->simulate(elapsedTime);
		mScene->fetchResults(true);
	}

	void WorldPhysX::AddBody(CollisionBody& body)
	{
		physx::PxActor* actor = Px::GetActor(body);
		mScene->addActor(*actor);
	}

	void WorldPhysX::RemoveBody(CollisionBody& body)
	{
		physx::PxActor* actor = Px::GetActor(body);
		mScene->removeActor(*actor);
	}

	std::vector<CollisionBody*> WorldPhysX::Overlap(const CollisionShape& shape, const Mat4& transform)
	{
		std::vector<CollisionBody*> bodies;
		physx::PxOverlapBufferN<128> buffer;
		physx::PxQueryFilterData filterData;
		filterData.flags |= physx::PxQueryFlag::eSTATIC | physx::PxQueryFlag::eDYNAMIC;
		const std::unique_ptr<physx::PxGeometry> geometry = Px::GetGeometry(shape);
		if (mScene->overlap(*geometry, Px::ToTransform(transform), buffer, filterData)) {
			for (physx::PxU32 i = 0; i < buffer.getNbTouches(); ++i) {
				const physx::PxOverlapHit& hit = buffer.getTouches()[i];
				if (const physx::PxRigidActor* actor = hit.actor) {
					bodies.push_back(static_cast<CollisionBody*>(actor->userData));
				}
			}
		}
		return bodies;
	}

	std::vector<CollisionBody*> WorldPhysX::RayCast(const Vec3& origin, const Vec3& direction, float distance)
	{
		std::vector<CollisionBody*> bodies;
		physx::PxRaycastBufferN<128> buffer;
		if (mScene->raycast(Px::ToVec3(origin), Px::ToVec3(direction), distance, buffer)) {
			for (physx::PxU32 i = 0; i < buffer.getNbAnyHits(); ++i) {
				const physx::PxRaycastHit& hit = buffer.getAnyHit(i);
				if (const physx::PxRigidActor* actor = hit.actor) {
					bodies.push_back(static_cast<CollisionBody*>(actor->userData));
				}
			}
		}
		return bodies;
	}
}