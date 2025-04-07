// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Core/FileSystem.h>
#include <ZeronTestFramework.h>

#include <DB/MapStorage.h>
#include <Platform/Platform.h>

using namespace ::Zeron;
using namespace ::Zeron::DB;

namespace TestModule_ZeronDB
{
	constexpr const char TEST_FOLDER[] = "./testData/";

	class MapStorageTests : public ::testing::Test {
	  public:
		void SetUp() override
		{
			mPlatform = Platform::Create({});
			mPlatform->Init();
			mPlatform->GetFileSystem().RemoveDir(TEST_FOLDER, true);
			ASSERT_FALSE(mPlatform->GetFileSystem().CreateDir(TEST_FOLDER).HasError());
		}

		void TearDown() override
		{
			ASSERT_FALSE(mPlatform->GetFileSystem().RemoveDir(TEST_FOLDER, true).HasError());
			mPlatform = nullptr;
		}

		std::unique_ptr<MapStorage> CreateDefaultStorage() { return MapStorage::Create({ .mPath = TEST_FOLDER, .mSize = 1L * 1024 * 1024 }); }


	  protected:
		std::unique_ptr<Platform> mPlatform;
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