// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Core/FileSystemError.h>
#include <Core/Types/ByteBuffer.h>
#include <Core/Types/Path.h>
#include <Core/Types/Result.h>

namespace Zeron
{
	class FileSystem {
	  public:
		virtual ~FileSystem() = default;
		virtual Result<ByteBuffer, FileSystemError> ReadBinaryFile(const Path& file) = 0;
		virtual Result<std::string, FileSystemError> ReadTextFile(const Path& file) = 0;
		virtual bool WriteBinaryFile(const Path& file, const ByteBuffer& data) = 0;
		virtual bool WriteTextFile(const Path& file, const std::string& data) = 0;

		virtual Result<void, FileSystemError> CreateDir(const Path& dir) = 0;
		virtual Result<void, FileSystemError> RemoveFile(const Path& file) = 0;
		virtual Result<void, FileSystemError> RemoveDir(const Path& dir, bool recursive = false) = 0;

		virtual bool FileExists(const Path& file) const = 0;
		virtual bool DirExists(const Path& dir) const = 0;
		virtual std::string ResolvePath(const Path& path) const = 0;
	};
}