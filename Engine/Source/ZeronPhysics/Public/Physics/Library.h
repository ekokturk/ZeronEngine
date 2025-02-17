// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron::Physics
{
	struct CollisionBodyConfig;
	class CollisionBody;
	struct DynamicBodyConfig;
	class Material;
	class World;
	struct WorldSettings;
	class DynamicBody;

	class Library {
	  public:
		virtual ~Library(){};

		virtual std::unique_ptr<World> CreateWorld(const WorldSettings& settings) = 0;
		virtual std::unique_ptr<CollisionBody> CreateStaticBody(const CollisionBodyConfig& config) = 0;
		virtual std::unique_ptr<DynamicBody> CreateDynamicBody(const DynamicBodyConfig& config) = 0;
		virtual std::unique_ptr<Material> CreateMaterial(float staticFriction, float dynamicFriction, float bounciness) = 0;
	};
}