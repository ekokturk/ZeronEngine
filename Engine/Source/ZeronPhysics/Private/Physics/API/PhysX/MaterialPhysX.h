// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Physics/Material.h>

namespace physx
{
	class PxMaterial;
}
namespace Zeron::Physics
{
	class LibraryPhysX;

	class MaterialPhysX : public Material {
	  public:
		MaterialPhysX(LibraryPhysX& library, float staticFriction, float dynamicFriction, float bounciness);
		~MaterialPhysX();

		void SetStaticFriction(float friction) override;
		void SetDynamicFriction(float friction) override;
		void SetBounciness(float bounciness) override;
		float GetStaticFriction() const override;
		float GetDynamicFriction() const override;
		float GetBounciness() const override;

		// PhysX
		physx::PxMaterial& GetMaterialPX();

	  private:
		physx::PxMaterial* mMaterial;
	};
}