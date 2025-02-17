// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Physics/API/PhysX/DynamicBodyPhysX.h>

#include <extensions/PxSimpleFactory.h>
#include <geometry/PxBoxGeometry.h>
#include <geometry/PxGeometry.h>
#include <Physics/API/PhysX/LibraryPhysX.h>
#include <Physics/API/PhysX/MaterialPhysX.h>
#include <Physics/API/PhysX/PhysXHelpers.h>
#include <PxActor.h>
#include <PxRigidDynamic.h>

namespace Zeron::Physics
{

	DynamicBodyPhysX::DynamicBodyPhysX(LibraryPhysX& library, const DynamicBodyConfig& config)
		: mMaterial(static_cast<MaterialPhysX*>(config.mMaterial))
	{
		mId = config.mId;
		physx::PxMaterial& mat = mMaterial ? mMaterial->GetMaterialPX() : library.GetDefaultMaterial();
		const physx::PxTransform transform = Px::ToTransform(config.mTransform);
		const std::unique_ptr<physx::PxGeometry> geo = Px::GetGeometry(config.mShape);
		mBody = PxCreateDynamic(library.GetPhysicsPX(), transform, *geo, mat, config.mMass);
		ZE_ASSERT(mBody, "RigidBodyPhysX: Could not create rigid body.");
		mBody->userData = this;
	}

	DynamicBodyPhysX::~DynamicBodyPhysX()
	{
		mBody->release();
	}

	Mat4 DynamicBodyPhysX::GetTransform() const
	{
		return Px::FromTransform(mBody->getGlobalPose());
	}

	Vec3 DynamicBodyPhysX::GetPosition() const
	{
		const auto pos = mBody->getGlobalPose().p;
		return { pos.x, pos.y, pos.z };
	}

	void DynamicBodyPhysX::SetLinearDamping(float damping)
	{
		mBody->setLinearDamping(damping);
	}

	void DynamicBodyPhysX::SetAngularDamping(float damping)
	{
		mBody->setAngularDamping(damping);
	}

	void DynamicBodyPhysX::SetLinearVelocity(const Vec3& vel)
	{
		mBody->setLinearVelocity(Px::ToVec3(vel));
	}

	void DynamicBodyPhysX::SetAngularVelocity(const Vec3& vel)
	{
		mBody->setAngularVelocity(Px::ToVec3(vel));
	}

	void DynamicBodyPhysX::ApplyForce(const Vec3& force)
	{
		mBody->addForce(Px::ToVec3(force));
	}

	void DynamicBodyPhysX::ApplyImpulse(const Vec3& impulse)
	{
		mBody->addForce(Px::ToVec3(impulse), physx::PxForceMode::eIMPULSE);
	}

	void DynamicBodyPhysX::ApplyTorque(const Vec3& torque)
	{
		mBody->addTorque(Px::ToVec3(torque));
	}

	void DynamicBodyPhysX::SetGravityEnabled(bool enabled)
	{
		mBody->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !enabled);
	}

	float DynamicBodyPhysX::GetLinearDamping() const
	{
		return mBody->getLinearDamping();
	}
	float DynamicBodyPhysX::GetAngularDamping() const
	{
		return mBody->getAngularDamping();
	}

	Vec3 DynamicBodyPhysX::GetVelocity() const
	{
		return Px::FromVec3(mBody->getLinearVelocity());
	}

	Vec3 DynamicBodyPhysX::GetAngularVelocity() const
	{
		return Px::FromVec3(mBody->getAngularVelocity());
	}

	bool DynamicBodyPhysX::IsGravityEnabled() const
	{
		return (mBody->getActorFlags() & physx::PxActorFlag::eDISABLE_GRAVITY) != physx::PxActorFlag::eDISABLE_GRAVITY;
	}

	float DynamicBodyPhysX::GetMass() const
	{
		return mBody->getMass();
	}


	physx::PxRigidDynamic& DynamicBodyPhysX::GetRigidPX()
	{
		return *mBody;
	}
}