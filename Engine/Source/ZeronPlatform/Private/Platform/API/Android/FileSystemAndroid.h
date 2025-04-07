// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_PLATFORM_ANDROID

#	include <Platform/API/Shared/FileSystemStandard.h>

struct AAssetManager;

namespace Zeron
{
	class FileSystemAndroid : public FileSystemStandard {
	  public:
		FileSystemAndroid(AAssetManager* assetManager);
		~FileSystemAndroid();

		Result<ByteBuffer, FileSystemError> ReadBinaryFile(const Path& file) override;
		Result<std::string, FileSystemError> ReadTextFile(const Path& file) override;
		bool WriteBinaryFile(const Path& file, const ByteBuffer& data) override;
		bool WriteTextFile(const Path& file, const std::string& data) override;

	  private:
		AAssetManager* mAssetManager;
	};
}

#endif