// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Physics/DynamicBody.h>

namespace physx
{
	class PxActor;
	class PxRigidDynamic;
}

namespace Zeron::Physics
{
	class MaterialPhysX;
	class LibraryPhysX;

	class DynamicBodyPhysX : public DynamicBody {
	  public:
		DynamicBodyPhysX(LibraryPhysX& library, const DynamicBodyConfig& config);
		~DynamicBodyPhysX() override;

		Mat4 GetTransform() const override;
		Vec3 GetPosition() const override;

		void SetLinearDamping(float damping) override;
		void SetAngularDamping(float damping) override;
		void SetLinearVelocity(const Vec3& vel) override;
		void SetAngularVelocity(const Vec3& vel) override;
		void ApplyForce(const Vec3& force) override;
		void ApplyImpulse(const Vec3& impulse) override;
		void ApplyTorque(const Vec3& torque) override;
		void SetGravityEnabled(bool enabled) override;

		float GetLinearDamping() const override;
		float GetAngularDamping() const override;
		Vec3 GetVelocity() const override;
		Vec3 GetAngularVelocity() const override;
		bool IsGravityEnabled() const override;
		float GetMass() const override;

		// PhysX
		physx::PxRigidDynamic& GetRigidPX();

	  private:
		MaterialPhysX* mMaterial;
		physx::PxRigidDynamic* mBody;
		Mat4 mTransform;
	};
}