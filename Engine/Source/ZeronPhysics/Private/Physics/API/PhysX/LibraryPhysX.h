// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Physics/Library.h>

#include <common/PxTolerancesScale.h>
#include <foundation/PxErrorCallback.h>

namespace physx
{
	class PxMaterial;
	class PxFoundation;
	class PxPhysics;
}

namespace Zeron::Physics
{
	struct CollisionBodyConfig;
	class CollisionBody;
	class MaterialPhysX;
	struct WorldSettings;
	class World;

	class LibraryPhysX : public Library {
	  public:
		LibraryPhysX();
		~LibraryPhysX() override;

		std::unique_ptr<World> CreateWorld(const WorldSettings& settings) override;
		std::unique_ptr<CollisionBody> CreateStaticBody(const CollisionBodyConfig& config) override;
		std::unique_ptr<DynamicBody> CreateDynamicBody(const DynamicBodyConfig& config) override;
		std::unique_ptr<Material> CreateMaterial(float staticFriction, float dynamicFriction, float bounciness) override;

		// PhysX
		physx::PxPhysics& GetPhysicsPX();
		physx::PxMaterial& GetDefaultMaterial();
		const physx::PxTolerancesScale& GetToleranceScalePX() const;

	  private:
		class PxDefaultErrorCallback : public physx::PxErrorCallback {
		  public:
			void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override;
		};

		struct Instance {
			physx::PxFoundation* CreateRef();
			void DestroyRef();

		  private:
			std::unique_ptr<physx::PxAllocatorCallback> mAllocatorCallback;
			PxDefaultErrorCallback mErrorCallback;
			physx::PxFoundation* mFoundation = nullptr;
			size_t mRefCount = 0;
		};

		static Instance mInstance;
		physx::PxFoundation* mFoundation;
		physx::PxTolerancesScale mToleranceScale;
		physx::PxPhysics* mPhysics;
		std::unique_ptr<MaterialPhysX> mDefaultMaterial;
	};
}