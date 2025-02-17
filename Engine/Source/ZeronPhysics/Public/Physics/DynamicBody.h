// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Physics/CollisionBody.h>
#include <Physics/CollisionShape.h>

namespace Zeron::Physics
{
	class Material;

	struct DynamicBodyConfig {
		int mId = -1;
		float mMass = 1.f;	// Kilograms
		Mat4 mTransform;
		Material* mMaterial = nullptr;
		CollisionShape mShape = BoxShape{};
	};

	class DynamicBody : public CollisionBody {
	  public:
		CollisionType GetCollisionType() const override { return CollisionType::Dynamic; }

		virtual void SetLinearDamping(float damping) = 0;
		virtual void SetAngularDamping(float damping) = 0;
		virtual void SetLinearVelocity(const Vec3& vel) = 0;
		virtual void SetAngularVelocity(const Vec3& vel) = 0;
		virtual void ApplyForce(const Vec3& force) = 0;
		virtual void ApplyImpulse(const Vec3& impulse) = 0;
		virtual void ApplyTorque(const Vec3& torque) = 0;
		virtual void SetGravityEnabled(bool enabled) = 0;

		virtual float GetLinearDamping() const = 0;
		virtual float GetAngularDamping() const = 0;
		virtual Vec3 GetVelocity() const = 0;
		virtual Vec3 GetAngularVelocity() const = 0;
		virtual bool IsGravityEnabled() const = 0;
		virtual float GetMass() const = 0;
	};
}