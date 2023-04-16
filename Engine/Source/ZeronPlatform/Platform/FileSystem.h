// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Platform/FileSystemHandler.h>

namespace Zeron
{
	class FileSystem {
	  public:
		static bool Init(IFileSystemHandler* handler);
		static Result<ByteBuffer, FileSystemError> ReadBinaryFile(const Path& file);
		static Result<std::string, FileSystemError> ReadTextFile(const Path& file);
		static bool WriteBinaryFile(const Path& file, const ByteBuffer& data);
		static bool WriteTextFile(const Path& file, const std::string& data);

	  protected:
		static IFileSystemHandler* mHandler;
	};
}