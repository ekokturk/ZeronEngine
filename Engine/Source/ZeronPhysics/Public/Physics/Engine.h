// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron::Physics
{
	class DynamicBody;
	class CollisionBody;
	struct CollisionBodyConfig;
	class World;
	struct WorldSettings;
	struct DynamicBodyConfig;
	class Material;
	class Library;
	class RigidBody;

	class Engine {
	  public:
		Engine();
		~Engine();

		virtual std::unique_ptr<World> CreateWorld(const WorldSettings& settings);
		virtual std::unique_ptr<CollisionBody> CreateStaticBody(const CollisionBodyConfig& config);
		virtual std::unique_ptr<DynamicBody> CreateDynamicBody(const DynamicBodyConfig& config);
		virtual std::unique_ptr<Material> CreateMaterial(float staticFriction = 0.5f, float dynamicFriction = 0.5f, float bounciness = 0.f);

	  private:
		std::unique_ptr<Library> mLibrary;
	};
}