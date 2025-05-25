// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <ZeronTestFramework.h>

#include <Entity/EntityManager.h>

using namespace ::Zeron;

namespace ZeronCoreTests::EntityTests
{
	struct Comp1 {
		int prop = 0;
	};
	struct Comp2 {
		float prop = 0.f;
	};
	struct Comp3 {
		std::string prop = "0";
	};

	std::array<Entity, 4> InitEntitiesWithComponentsFixture(EntityManager& manager)
	{
		const Entity e1 = manager.CreateEntity();
		const Entity e2 = manager.CreateEntity();
		const Entity e3 = manager.CreateEntity();
		const Entity e4 = manager.CreateEntity();

		manager.AddComponent<Comp2>(e1);
		manager.AddComponent<Comp1>(e2);
		manager.AddComponent<Comp2>(e2);
		manager.AddComponent<Comp3>(e2);
		manager.AddComponent<Comp2>(e3);
		manager.AddComponent<Comp3>(e3);
		manager.AddComponent<Comp1>(e4);
		manager.AddComponent<Comp3>(e4);

		return { e1, e2, e3, e4 };
	}

	TEST(EntityManagerTests, CreateEntities_RemoveFirstAndAddNew_HasRegisteredEntities)
	{
		EntityManager manager;
		const Entity e1 = manager.CreateEntity();
		EXPECT_TRUE(manager.HasEntity(e1));
		const Entity e2 = manager.CreateEntity();
		EXPECT_TRUE(manager.HasEntity(e2));

		manager.RemoveEntity(e1);
		EXPECT_FALSE(manager.HasEntity(e1));

		const Entity e3 = manager.CreateEntity();
		EXPECT_FALSE(manager.HasEntity(e1));
		EXPECT_NE(e1, e3);
		EXPECT_EQ(e1.GetId(), e3.GetId());
	}

	TEST(EntityManagerTests, CreateEntityWithComponents_AddGetUpdateRemove_ComponentsModified)
	{
		EntityManager manager;
		const Entity e1 = manager.CreateEntity();
		EXPECT_TRUE(manager.HasEntity(e1));

		struct Comp1 {};
		struct Comp2 {
			int prop = 0;
		};

		// Add
		manager.AddComponent<Comp1>(e1);
		EXPECT_TRUE(manager.HasComponent<Comp1>(e1));
		manager.AddComponent<Comp2>(e1, Comp2{ 1 });
		EXPECT_TRUE(manager.HasComponent<Comp2>(e1));

		// Get / Update
		Comp2& compRef = manager.GetComponent<Comp2>(e1);
		EXPECT_EQ(1, compRef.prop);
		compRef.prop = 10;
		EXPECT_EQ(10, compRef.prop);
		Comp2* compPtr = manager.TryGetComponent<Comp2>(e1);
		EXPECT_NE(nullptr, compPtr);
		EXPECT_EQ(10, compPtr->prop);
		compPtr->prop = 20;
		const Comp2& compConstRef = manager.GetComponent<Comp2>(e1);
		EXPECT_EQ(20, compConstRef.prop);

		// Remove
		manager.RemoveComponent<Comp1>(e1);
		EXPECT_FALSE(manager.HasComponent<Comp1>(e1));
		manager.RemoveComponent<Comp2>(e1);
		EXPECT_FALSE(manager.HasComponent<Comp2>(e1));
		Comp2* compPtrRemoved = manager.TryGetComponent<Comp2>(e1);
		EXPECT_EQ(nullptr, compPtrRemoved);
	}

	TEST(EntityManagerTests, CreateEntitiesWithComponents_ForEachWithAllComponents_CallbacksValid)
	{
		EntityManager manager;
		const std::array<Entity, 4> ent = InitEntitiesWithComponentsFixture(manager);

		// Iterate for All Three Components
		int triggerCount = 0;
		manager.ForEach<Comp1, Comp2, Comp3>([&](Entity entity, Comp1& comp1, Comp2& comp2, Comp3& comp3) {
			++triggerCount;
			ASSERT_EQ(ent[1], entity);
			comp1.prop = 1;
			comp2.prop = 1.f;
			comp3.prop = "1";
		});
		ASSERT_EQ(1, triggerCount);
		ASSERT_EQ(1, manager.GetComponent<Comp1>(ent[1]).prop);
		ASSERT_FLOAT_EQ(1.f, manager.GetComponent<Comp2>(ent[1]).prop);
		ASSERT_EQ("1", manager.GetComponent<Comp3>(ent[1]).prop);

		triggerCount = 0;
		manager.ForEach<Comp1, Comp2, Comp3>([&](Comp1& comp1, Comp2& comp2, Comp3& comp3) {
			++triggerCount;
			comp1.prop = 2;
			comp2.prop = 2.f;
			comp3.prop = "2";
		});
		ASSERT_EQ(1, triggerCount);
		ASSERT_EQ(2, manager.GetComponent<Comp1>(ent[1]).prop);
		ASSERT_FLOAT_EQ(2.f, manager.GetComponent<Comp2>(ent[1]).prop);
		ASSERT_EQ("2", manager.GetComponent<Comp3>(ent[1]).prop);

		triggerCount = 0;
		manager.ForEach<Comp1, Comp2, Comp3>([&](Entity entity) {
			++triggerCount;
			ASSERT_EQ(ent[1], entity);
		});
		ASSERT_EQ(1, triggerCount);
	}

	TEST(EntityManagerTests, CreateEntitiesWithComponents_ForEachWithSomeComponents_CallbacksValid)
	{
		EntityManager manager;
		const std::array<Entity, 4> ent = InitEntitiesWithComponentsFixture(manager);

		// Iterate for Two Components
		int triggerCount = 0;
		manager.ForEach<Comp2, Comp3>([&](Entity entity, Comp2& comp2, Comp3& comp3) {
			ASSERT_TRUE(ent[1] == entity || ent[2] == entity);
			++triggerCount;
			comp2.prop = 1.f;
			comp3.prop = "1";
		});
		ASSERT_EQ(2, triggerCount);
		ASSERT_FLOAT_EQ(1.f, manager.GetComponent<Comp2>(ent[1]).prop);
		ASSERT_EQ("1", manager.GetComponent<Comp3>(ent[1]).prop);
		ASSERT_FLOAT_EQ(1.f, manager.GetComponent<Comp2>(ent[2]).prop);
		ASSERT_EQ("1", manager.GetComponent<Comp3>(ent[2]).prop);

		triggerCount = 0;
		manager.ForEach<Comp2, Comp3>([&](Comp2& comp2, Comp3& comp3) {
			++triggerCount;
			comp2.prop = 2.f;
			comp3.prop = "2";
		});
		ASSERT_EQ(2, triggerCount);
		ASSERT_FLOAT_EQ(2.f, manager.GetComponent<Comp2>(ent[1]).prop);
		ASSERT_EQ("2", manager.GetComponent<Comp3>(ent[1]).prop);
		ASSERT_FLOAT_EQ(2.f, manager.GetComponent<Comp2>(ent[2]).prop);
		ASSERT_EQ("2", manager.GetComponent<Comp3>(ent[2]).prop);

		triggerCount = 0;
		manager.ForEach<Comp2, Comp3>([&](Entity entity) {
			ASSERT_TRUE(ent[1] == entity || ent[2] == entity);
			++triggerCount;
		});
		ASSERT_EQ(2, triggerCount);
	}

	TEST(EntityManagerTests, CreateEntitiesWithComponents_ForEachWithSingleComponent_CallbacksValid)
	{
		EntityManager manager;
		const std::array<Entity, 4> ent = InitEntitiesWithComponentsFixture(manager);

		// Iterate for Single Component
		int triggerCount = 0;
		manager.ForEach<Comp3>([&](Entity entity, Comp3& comp3) {
			ASSERT_TRUE(ent[1] == entity || ent[2] == entity || ent[3] == entity);
			++triggerCount;
			comp3.prop = "1";
		});
		ASSERT_EQ(3, triggerCount);
		ASSERT_EQ("1", manager.GetComponent<Comp3>(ent[1]).prop);
		ASSERT_EQ("1", manager.GetComponent<Comp3>(ent[2]).prop);
		ASSERT_EQ("1", manager.GetComponent<Comp3>(ent[3]).prop);

		triggerCount = 0;
		manager.ForEach<Comp3>([&](Comp3& comp3) {
			++triggerCount;
			comp3.prop = "2";
		});
		ASSERT_EQ(3, triggerCount);
		ASSERT_EQ("2", manager.GetComponent<Comp3>(ent[1]).prop);
		ASSERT_EQ("2", manager.GetComponent<Comp3>(ent[2]).prop);
		ASSERT_EQ("2", manager.GetComponent<Comp3>(ent[3]).prop);

		triggerCount = 0;
		manager.ForEach<Comp3>([&](Entity entity) {
			ASSERT_TRUE(ent[1] == entity || ent[2] == entity || ent[3] == entity);
			++triggerCount;
		});
		ASSERT_EQ(3, triggerCount);
	}

	TEST(EntityManagerTests, CreateEntitiesWithComponents_ForEachWithNoComponent_CallbacksValid)
	{
		EntityManager manager;
		const std::array<Entity, 4> ent = InitEntitiesWithComponentsFixture(manager);

		// Iterate all entities
		int triggerCount = 0;
		manager.ForEach([&](Entity entity) {
			++triggerCount;
		});

		ASSERT_EQ(4, triggerCount);
	}

	TEST(EntityManagerViewTests, CreateEntitiesWithComponents_IterateAndUpdateSingleComponent_ModifiedComponent)
	{
		EntityManager manager;
		const std::array<Entity, 4> ent = InitEntitiesWithComponentsFixture(manager);

		auto view1 = EntityManager::View<Comp1>(manager);
		for (auto [entity, comp1] : view1) {
			ASSERT_TRUE(ent[1] == entity || ent[3] == entity);
			comp1.prop = 1;
		}

		ASSERT_EQ(1, manager.GetComponent<Comp1>(ent[1]).prop);
		ASSERT_EQ(1, manager.GetComponent<Comp1>(ent[3]).prop);
	}

	TEST(EntityManagerViewTests, CreateEntitiesWithComponents_IterateAndUpdateMultipleComponents_ModifiedComponents)
	{
		EntityManager manager;
		const std::array<Entity, 4> ent = InitEntitiesWithComponentsFixture(manager);

		auto view2 = EntityManager::View<Comp2, Comp3>(manager);
		for (auto&& [entity, comp2, comp3] : view2) {
			ASSERT_TRUE(ent[1] == entity || ent[2] == entity);
			comp2.prop = 1.f;
			comp3.prop = "1";
		}

		ASSERT_FLOAT_EQ(1.f, manager.GetComponent<Comp2>(ent[1]).prop);
		ASSERT_FLOAT_EQ(1.f, manager.GetComponent<Comp2>(ent[2]).prop);
		ASSERT_EQ("1", manager.GetComponent<Comp3>(ent[1]).prop);
		ASSERT_EQ("1", manager.GetComponent<Comp3>(ent[2]).prop);
	}
}