// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Platform/FileSystemHandler.h>

namespace Zeron
{
	class FileSystemHandlerStandard : public IFileSystemHandler {
	public:
		Result<ByteBuffer, FileSystemError> ReadBinaryFile(const Path& file) override;
		Result<std::string, FileSystemError> ReadTextFile(const Path& file) override;
		bool WriteBinaryFile(const Path& file, const ByteBuffer& data) override;
		bool WriteTextFile(const Path& file, const std::string& data) override;
	};
}
