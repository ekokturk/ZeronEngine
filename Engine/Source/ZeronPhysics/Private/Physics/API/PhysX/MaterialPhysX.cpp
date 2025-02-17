// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Physics/API/PhysX/MaterialPhysX.h>

#include <Physics/API/PhysX/LibraryPhysX.h>
#include <PxMaterial.h>
#include <PxPhysics.h>

namespace Zeron::Physics
{

	MaterialPhysX::MaterialPhysX(LibraryPhysX& library, float staticFriction, float dynamicFriction, float bounciness)
	{
		mMaterial = library.GetPhysicsPX().createMaterial(staticFriction, dynamicFriction, bounciness);
		ZE_ASSERT(mMaterial, "MaterialPhysX: Could not create physics material.");
	}

	MaterialPhysX::~MaterialPhysX()
	{
		mMaterial->release();
	}

	void MaterialPhysX::SetStaticFriction(float friction)
	{
		mMaterial->setStaticFriction(friction);
	}

	void MaterialPhysX::SetDynamicFriction(float friction)
	{
		mMaterial->setDynamicFriction(friction);
	}

	void MaterialPhysX::SetBounciness(float bounciness)
	{
		mMaterial->setRestitution(bounciness);
	}

	float MaterialPhysX::GetStaticFriction() const
	{
		return mMaterial->getStaticFriction();
	}

	float MaterialPhysX::GetDynamicFriction() const
	{
		return mMaterial->getDynamicFriction();
	}

	float MaterialPhysX::GetBounciness() const
	{
		return mMaterial->getRestitution();
	}

	physx::PxMaterial& MaterialPhysX::GetMaterialPX()
	{
		ZE_ASSERT(mMaterial, "MaterialPhysX: Expected material to exist.");
		return *mMaterial;
	}
}