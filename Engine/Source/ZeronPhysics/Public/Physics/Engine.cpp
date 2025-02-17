// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Physics/Engine.h>

#include <Physics/API/PhysX/LibraryPhysX.h>
#include <Physics/CollisionBody.h>
#include <Physics/DynamicBody.h>
#include <Physics/Library.h>
#include <Physics/Material.h>
#include <Physics/World.h>

namespace Zeron::Physics
{
	Engine::Engine()
	{
		mLibrary = std::make_unique<LibraryPhysX>();
	}

	Engine::~Engine() {}

	std::unique_ptr<World> Engine::CreateWorld(const WorldSettings& settings)
	{
		return mLibrary->CreateWorld(settings);
	}

	std::unique_ptr<CollisionBody> Engine::CreateStaticBody(const CollisionBodyConfig& config)
	{
		return mLibrary->CreateStaticBody(config);
	}

	std::unique_ptr<DynamicBody> Engine::CreateDynamicBody(const DynamicBodyConfig& config)
	{
		return mLibrary->CreateDynamicBody(config);
	}

	std::unique_ptr<Material> Engine::CreateMaterial(float staticFriction, float dynamicFriction, float bounciness)
	{
		return mLibrary->CreateMaterial(staticFriction, dynamicFriction, bounciness);
	}

}