// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Core/FileSystem.h>
#include <ZeronTestFramework.h>

#include <Platform/Platform.h>

using namespace ::Zeron;

namespace ZeronPlatformTests
{
	class FileSystemTest : public ::testing::Test {
	  public:
		void SetUp() override { ASSERT_TRUE(Locator::Get<Platform>()); }
		void TearDown() override {}

		FileSystem& GetFileSystem() { return Locator::Get<Platform>()->GetFileSystem(); }
	};


	TEST_F(FileSystemTest, File_WriteTextReadThenRemove_OperationsAreValid)
	{
		auto& fs = GetFileSystem();
		const Path filePath = "./testfile";
		const std::string fileContents = "My Test file contents";

		ASSERT_FALSE(fs.FileExists(filePath));
		ASSERT_TRUE(fs.WriteTextFile(filePath, fileContents));
		ASSERT_TRUE(fs.FileExists(filePath));
		const auto readResult = fs.ReadTextFile(filePath);
		ASSERT_TRUE(fs.RemoveFile(filePath));

		ASSERT_FALSE(readResult.HasError());
		ASSERT_EQ(fileContents, readResult.Value());
		ASSERT_FALSE(fs.FileExists(filePath));
	}

	TEST_F(FileSystemTest, File_WriteBinaryReadThenRemove_OperationsAreValid)
	{
		auto& fs = GetFileSystem();
		const Path filePath = "./testfile";
		struct Payload {
			int mInt = 0;
			float mFloat = 0;
		};
		ByteBuffer buffer;
		buffer.Append<Payload>({ -12, 34.56f });

		ASSERT_FALSE(fs.FileExists(filePath));
		ASSERT_TRUE(fs.WriteBinaryFile(filePath, buffer));
		ASSERT_TRUE(fs.FileExists(filePath));
		const auto readResult = fs.ReadBinaryFile(filePath);
		const ByteBuffer& readBuffer = readResult.Value();
		ASSERT_TRUE(fs.RemoveFile(filePath));

		ASSERT_FALSE(readResult.HasError());
		ASSERT_TRUE(!readBuffer.empty());
		Payload readPayload;
		readBuffer.CopyTo(readPayload);
		ASSERT_EQ(-12, readPayload.mInt);
		ASSERT_EQ(34.56f, readPayload.mFloat);
		ASSERT_FALSE(fs.FileExists(filePath));
	}

	TEST_F(FileSystemTest, Directory_CreateThenRemove_OperationsAreValid)
	{
		auto& fs = GetFileSystem();
		const Path dirPath = "./testDir";

		ASSERT_FALSE(fs.DirExists(dirPath));
		ASSERT_TRUE(fs.CreateDir(dirPath));
		ASSERT_TRUE(fs.DirExists(dirPath));
		ASSERT_TRUE(fs.RemoveDir(dirPath));

		ASSERT_FALSE(fs.DirExists(dirPath));
	}

	TEST_F(FileSystemTest, Directory_CreateWithSubDirThenRemove_OperationsAreValid)
	{
		auto& fs = GetFileSystem();
		const Path dirPath = "./testDir";
		const Path subDirPath = dirPath / "subDir";

		ASSERT_FALSE(fs.DirExists(subDirPath));
		ASSERT_TRUE(fs.CreateDir(subDirPath));
		ASSERT_TRUE(fs.DirExists(subDirPath));
		ASSERT_FALSE(fs.RemoveDir(dirPath));
		ASSERT_TRUE(fs.DirExists(dirPath));
		ASSERT_TRUE(fs.RemoveDir(dirPath, true));

		ASSERT_FALSE(fs.DirExists(dirPath));
	}
}