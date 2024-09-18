// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Platform/FileSystemError.h>

namespace Zeron
{
	class IFileSystemHandler {
	  public:
		virtual ~IFileSystemHandler() = default;
		virtual Result<ByteBuffer, FileSystemError> ReadBinaryFile(const Path& file) = 0;
		virtual Result<std::string, FileSystemError> ReadTextFile(const Path& file) = 0;
		virtual bool WriteBinaryFile(const Path& file, const ByteBuffer& data) = 0;
		virtual bool WriteTextFile(const Path& file, const std::string& data) = 0;
	};
}