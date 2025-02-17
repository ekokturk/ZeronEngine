// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <ZeronTestFramework.h>

#include <Physics/CollisionBody.h>
#include <Physics/DynamicBody.h>
#include <Physics/Engine.h>
#include <Physics/World.h>

using namespace ::Zeron;
using namespace ::Zeron::Physics;

namespace TestModule_ZeronPhysics::EntityTests
{

	TEST(PhysicsTests, CreateDynamicBody_HasCorrectAttributes)
	{
		Engine engine;
		const Vec3 pos = Vec3(10.f, 20.f, 30.f);
		const Vec3 rot = Vec3(45.f, 60.f, 25.f);
		const DynamicBodyConfig config{
			.mId = 20,
			.mMass = 10,
			.mTransform = Math::MakeTransform(pos, Math::ToRadians(rot)),
			.mShape = BoxShape{},
		};
		const auto body = engine.CreateDynamicBody(config);

		EXPECT_EQ(body->GetId(), 20);
		EXPECT_EQ(body->GetCollisionType(), CollisionType::Dynamic);
		EXPECT_EQ(body->GetMass(), 10.0f);
		EXPECT_TRUE(body->IsGravityEnabled());
		const Mat4 transform = body->GetTransform();
		EXPECT_VEC3_NEAR(pos, Math::GetPos(transform), 0.01f)
		EXPECT_VEC3_NEAR(rot, Math::ToDegrees(Math::GetEulerAngles(transform)), 0.01f)
		EXPECT_QUAT_NEAR(Math::GetQuat(config.mTransform), Math::GetQuat(transform), 0.01f);
		EXPECT_VEC3_NEAR(pos, body->GetPosition(), 0.01f);
		EXPECT_VEC3_NEAR(Vec3::ZERO, body->GetVelocity(), 0.01f);
	}

	TEST(PhysicsTests, CreateStaticBody_HasCorrectAttributes)
	{
		Engine engine;
		const Vec3 pos = Vec3(-10.f, 20.1f, -12.f);
		const Vec3 rot = Vec3(-45.f, 60.f, -75.f);
		const CollisionBodyConfig config{ .mId = 12, .mTransform = Math::MakeTransform(pos, Math::ToRadians(rot)), .mShape = BoxShape{} };
		const auto body = engine.CreateStaticBody(config);

		EXPECT_EQ(body->GetId(), 12);
		EXPECT_EQ(body->GetCollisionType(), CollisionType::Static);
		const Mat4 transform = body->GetTransform();
		EXPECT_VEC3_NEAR(pos, Math::GetPos(transform), 0.01f)
		EXPECT_VEC3_NEAR(rot, Math::ToDegrees(Math::GetEulerAngles(transform)), 0.01f)
		EXPECT_QUAT_NEAR(Math::GetQuat(config.mTransform), Math::GetQuat(transform), 0.01f);
		EXPECT_VEC3_NEAR(pos, body->GetPosition(), 0.01f);
	}

	TEST(PhysicsTests, CreateDynamicBody_ApplyForce_BodyMoved)
	{
		Engine engine;
		const auto world = engine.CreateWorld(WorldSettings{});
		const DynamicBodyConfig config{ .mMass = 10 };
		const auto body = engine.CreateDynamicBody(config);
		world->AddBody(*body);

		body->ApplyForce({ 10, 10, 20 });
		world->Simulate(1.0f);

		const Vec3 vel = body->GetVelocity();
		EXPECT_VEC3_NEAR(Vec3(1.0f, -8.81f, 2.0f), vel, 0.01f);
	}

	TEST(PhysicsTests, CreateDynamicBody_ApplyTorque_BodyMoved)
	{
		Engine engine;
		const auto world = engine.CreateWorld(WorldSettings{});
		const DynamicBodyConfig config{ .mMass = 10 };
		const auto body = engine.CreateDynamicBody(config);
		body->SetAngularDamping(0);
		world->AddBody(*body);

		body->ApplyTorque({ 5, 10, 20 });
		world->Simulate(1.0f);

		EXPECT_VEC3_NEAR(Vec3(3.f, 6.f, 12.f), body->GetAngularVelocity(), 0.01f);
	}

	TEST(PhysicsTests, CreateDynamicBody_WaitForGravity_BodyMoved)
	{
		Engine engine;
		const auto world = engine.CreateWorld(WorldSettings{});
		const DynamicBodyConfig config{ .mMass = 10, .mTransform = Math::Translate(Mat4{}, Vec3{ 0, 10.f, 0 }) };
		const auto body = engine.CreateDynamicBody(config);
		world->AddBody(*body);

		world->Simulate(1.0f);

		EXPECT_NEAR(body->GetVelocity().Y, -9.81f, 0.01f);
	}

	TEST(PhysicsTests, CreateDynamicBody_WaitForCustomGravity_BodyMoved)
	{
		Engine engine;
		const auto world = engine.CreateWorld(WorldSettings{ .mGravity = { -1.1f, -5.f, 2.4f } });
		const DynamicBodyConfig config{ .mMass = 10, .mTransform = Math::Translate(Mat4{}, Vec3{ 0, 10.f, 0 }) };
		const auto body = engine.CreateDynamicBody(config);
		world->AddBody(*body);

		world->Simulate(1.0f);

		EXPECT_VEC3_NEAR(Vec3(-1.1f, -5.f, 2.4f), body->GetVelocity(), 0.01f);
	}

	TEST(PhysicsTests, CreateBodies_Overlap_FoundBodies)
	{
		Engine engine;
		const auto world = engine.CreateWorld(WorldSettings{});
		const auto staticBody = engine.CreateDynamicBody({ .mId = 10 });
		const auto dynamicBody = engine.CreateStaticBody({ .mId = 20 });
		world->AddBody(*staticBody);
		world->AddBody(*dynamicBody);

		const auto bodies = world->Overlap(SphereShape{ 10.f });

		ASSERT_EQ(2, bodies.size());
		bool foundStatic = false;
		bool foundDynamic = false;
		for (const auto& body : bodies) {
			if (body->GetId() == 10) {
				foundDynamic = true;
			}
			if (body->GetId() == 20) {
				foundStatic = true;
			}
		}
		EXPECT_TRUE(foundStatic);
		EXPECT_TRUE(foundDynamic);
	}

	TEST(PhysicsTests, CreateBodies_Raycast_FoundClosestBody)
	{
		Engine engine;
		const auto world = engine.CreateWorld(WorldSettings{});
		const auto staticBody = engine.CreateDynamicBody({ .mId = 10, .mTransform = Math::Translate(Mat4(), Vec3{ 10.f, 0.f, 0.f }) });
		const auto dynamicBody = engine.CreateStaticBody({ .mId = 20, .mTransform = Math::Translate(Mat4(), Vec3{ 20.f, 0.f, 0.f }) });
		world->AddBody(*staticBody);
		world->AddBody(*dynamicBody);

		const auto bodies = world->RayCast(Vec3{}, Vec3{ 1.f, 0, 0 }, 10.f);

		ASSERT_EQ(1, bodies.size());
		EXPECT_EQ(10, bodies[0]->GetId());
	}

}