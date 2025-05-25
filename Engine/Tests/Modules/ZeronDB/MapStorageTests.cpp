// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Core/FileSystem.h>
#include <ZeronTestFramework.h>

#include <DB/MapStorage.h>
#include <Platform/Platform.h>

using namespace ::Zeron;
using namespace ::Zeron::DB;

namespace ZeronDBTests
{
	constexpr const char TEST_FOLDER[] = "testData/";

	class MapStorageTests : public ::testing::Test {
	  public:
		void SetUp() override
		{
			ASSERT_TRUE(Locator::Get<Platform>());
			mPath = Locator::Get<Platform>()->GetFileSystem().ResolvePath(TEST_FOLDER);
			ASSERT_FALSE(Locator::Get<Platform>()->GetFileSystem().RemoveDir(mPath, true).HasError());
			ASSERT_FALSE(Locator::Get<Platform>()->GetFileSystem().CreateDir(mPath).HasError());
		}

		void TearDown() override { ASSERT_FALSE(Locator::Get<Platform>()->GetFileSystem().RemoveDir(mPath, true).HasError()); }

		std::unique_ptr<MapStorage> CreateDefaultStorage() { return MapStorage::Create({ .mPath = mPath, .mSize = 1L * 1024 * 1024 }); }

	  private:
		std::string mPath;
	};


	TEST_F(MapStorageTests, DefaultStorage_InsertThenGetValue_HasValue)
	{
		std::unique_ptr<MapStorage> db = CreateDefaultStorage();
		std::unique_ptr<Transaction> t = db->CreateTransaction();
		t->Put("Test", "true");
		t->Commit();

		std::unique_ptr<Transaction> t2 = db->CreateTransaction();
		const std::string expected = t2->Get("Test");
		t2->Commit();

		EXPECT_EQ(expected, "true");
	}

	TEST_F(MapStorageTests, DefaultStorage_InsertToOtherThenGetValue_HasValueInOther)
	{
		std::unique_ptr<MapStorage> db = CreateDefaultStorage();

		std::unique_ptr<Transaction> t1 = db->CreateTransaction("Other");
		t1->Put("Test", "true");
		t1->Commit();
		std::unique_ptr<Transaction> t2 = db->CreateTransaction();
		const std::string emptyVal = t2->Get("Test");
		t2->Commit();
		std::unique_ptr<Transaction> t3 = db->CreateTransaction("Other");
		const std::string expectedVal = t3->Get("Test");
		t3->Commit();

		EXPECT_EQ(emptyVal, "");
		EXPECT_EQ(expectedVal, "true");
	}

	TEST_F(MapStorageTests, DefaultStorage_InsertThenDeleteValue_HasNoValue)
	{
		std::unique_ptr<MapStorage> db = CreateDefaultStorage();

		std::unique_ptr<Transaction> t1 = db->CreateTransaction();
		t1->Put("Test", "true");
		t1->Commit();
		std::unique_ptr<Transaction> t2 = db->CreateTransaction();
		t2->Delete("Test");
		t2->Commit();
		std::unique_ptr<Transaction> t3 = db->CreateTransaction();
		const std::string expectedVal = t3->Get("Test");
		t3->Commit();

		EXPECT_EQ(expectedVal, "");
	}

	TEST_F(MapStorageTests, DefaultStorage_GetNonExistingValue_HasEmptyValue)
	{
		std::unique_ptr<MapStorage> db = CreateDefaultStorage();

		std::unique_ptr<Transaction> t2 = db->CreateTransaction();
		const std::string expected = t2->Get("Test");
		t2->Commit();

		EXPECT_EQ(expected, "");
	}


}