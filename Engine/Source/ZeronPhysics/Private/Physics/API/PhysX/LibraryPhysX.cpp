// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Physics/API/PhysX/LibraryPhysX.h>

#include <common/PxTolerancesScale.h>
#include <extensions/PxDefaultAllocator.h>
#include <foundation/PxFoundation.h>
#include <foundation/PxPhysicsVersion.h>
#include <Physics/API/PhysX/CollisionBodyPhysX.h>
#include <Physics/API/PhysX/DynamicBodyPhysX.h>
#include <Physics/API/PhysX/MaterialPhysX.h>
#include <Physics/API/PhysX/WorldPhysX.h>
#include <PxPhysics.h>

namespace Zeron::Physics
{
	LibraryPhysX::Instance LibraryPhysX::mInstance = {};

	LibraryPhysX::LibraryPhysX()
		: mFoundation(mInstance.CreateRef())
		, mPhysics(nullptr)
	{
		ZE_ASSERT(mFoundation, "LibraryPhysX: Couldn't create foundation.");
		mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale, true);
		ZE_ASSERT(mPhysics, "LibraryPhysX: Couldn't create physics SDK.");
		mDefaultMaterial = std::make_unique<MaterialPhysX>(*this, 0.5f, 0.5f, 0.f);
	}

	LibraryPhysX::~LibraryPhysX()
	{
		mDefaultMaterial = nullptr;
		mPhysics->release();
		mInstance.DestroyRef();
	}

	std::unique_ptr<World> LibraryPhysX::CreateWorld(const WorldSettings& settings)
	{
		return std::make_unique<WorldPhysX>(*this, settings);
	}

	std::unique_ptr<CollisionBody> LibraryPhysX::CreateStaticBody(const CollisionBodyConfig& config)
	{
		return std::make_unique<CollisionBodyPhysX>(*this, config);
	}

	std::unique_ptr<DynamicBody> LibraryPhysX::CreateDynamicBody(const DynamicBodyConfig& config)
	{
		return std::make_unique<DynamicBodyPhysX>(*this, config);
	}

	std::unique_ptr<Material> LibraryPhysX::CreateMaterial(float staticFriction, float dynamicFriction, float bounciness)
	{
		return std::make_unique<MaterialPhysX>(*this, staticFriction, dynamicFriction, bounciness);
	}

	physx::PxPhysics& LibraryPhysX::GetPhysicsPX()
	{
		return *mPhysics;
	}

	physx::PxMaterial& LibraryPhysX::GetDefaultMaterial()
	{
		return mDefaultMaterial->GetMaterialPX();
	}

	const physx::PxTolerancesScale& LibraryPhysX::GetToleranceScalePX() const
	{
		return mToleranceScale;
	}

	void LibraryPhysX::PxDefaultErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
	{
		ZE_LOG("PhysicsPhysX [ERROR]: {} (File: {}, Line: {})", message, file, line);
	}

	physx::PxFoundation* LibraryPhysX::Instance::CreateRef()
	{
		if (!mFoundation) {
			mAllocatorCallback = std::make_unique<physx::PxDefaultAllocator>();
			mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *mAllocatorCallback, mErrorCallback);
		}
		++mRefCount;
		return mFoundation;
	}

	void LibraryPhysX::Instance::DestroyRef()
	{
		--mRefCount;
		if (mRefCount == 0) {
			mFoundation->release();
			mAllocatorCallback = nullptr;
			mFoundation = nullptr;
		}
	}
}