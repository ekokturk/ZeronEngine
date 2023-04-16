// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Platform/FileSystem.h>

namespace Zeron
{
	IFileSystemHandler* FileSystem::mHandler = nullptr;

	bool FileSystem::Init(IFileSystemHandler* handler)
	{
		if (!mHandler) {
			mHandler = handler;
			return mHandler;
		}
		return false;
	}

	Result<ByteBuffer, FileSystemError> FileSystem::ReadBinaryFile(const Path& file)
	{
		return mHandler->ReadBinaryFile(file);
	}

	Result<std::string, FileSystemError> FileSystem::ReadTextFile(const Path& file)
	{
		return mHandler->ReadTextFile(file);
	}

	bool FileSystem::WriteBinaryFile(const Path& file, const ByteBuffer& data)
	{
		return mHandler->WriteBinaryFile(file, data);
	}

	bool FileSystem::WriteTextFile(const Path& file, const std::string& data)
	{
		return mHandler->WriteTextFile(file, data);
	}
}
