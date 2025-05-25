// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Core/FileSystem.h>

namespace Zeron
{
	class FileSystemStandard : public FileSystem {
	  public:
		Result<ByteBuffer, FileSystemError> ReadBinaryFile(const Path& file) override;
		Result<std::string, FileSystemError> ReadTextFile(const Path& file) override;
		bool WriteBinaryFile(const Path& file, const ByteBuffer& data) override;
		bool WriteTextFile(const Path& file, const std::string& data) override;
		Result<void, FileSystemError> CreateDir(const Path& dir) override;
		Result<void, FileSystemError> RemoveFile(const Path& file) override;
		Result<void, FileSystemError> RemoveDir(const Path& dir, bool recursive) override;
		bool FileExists(const Path& file) const override;
		bool DirExists(const Path& dir) const override;
		std::string ResolvePath(const Path& path) const override;
	};
}