// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Physics/CollisionShape.h>

namespace Zeron::Physics
{
	class CollisionBody;

	struct WorldSettings {
		Vec3 mGravity{ 0, -9.81f, 0 };
	};

	class World {
	  public:
		virtual ~World(){};

		virtual void AddBody(CollisionBody& body) = 0;
		virtual void RemoveBody(CollisionBody& body) = 0;
		virtual void Simulate(float elapsedTimeInSeconds) = 0;
		virtual std::vector<CollisionBody*> Overlap(const CollisionShape& shape, const Mat4& transform = Mat4()) = 0;
		virtual std::vector<CollisionBody*> RayCast(const Vec3& origin, const Vec3& direction, float distance) = 0;

	  protected:
		World(const WorldSettings& settings)
			: mSettings(settings)
		{}

		WorldSettings mSettings;
	};
}