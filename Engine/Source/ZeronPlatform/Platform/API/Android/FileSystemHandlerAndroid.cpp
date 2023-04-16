// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_PLATFORM_ANDROID

#include <Platform/API/Android/FileSystemHandlerAndroid.h>

#include <android/asset_manager.h>

namespace Zeron
{
	FileSystemHandlerAndroid::FileSystemHandlerAndroid(AAssetManager* assetManager) 
		: mAssetManager(assetManager)
	{
	}

	FileSystemHandlerAndroid::~FileSystemHandlerAndroid() 
	{
	}

	Result<ByteBuffer, FileSystemError> FileSystemHandlerAndroid::ReadBinaryFile(const Path& file)
	{
		// Try finding file in android assets first if path is relative
		if (file.IsRelative()) {
			AAsset* androidAsset = AAssetManager_open(mAssetManager, file.ToString().c_str(), AASSET_MODE_BUFFER);
			if (androidAsset) {
				const size_t fileLength = AAsset_getLength(androidAsset);
				ByteBuffer data(fileLength);
				int64_t readSize = AAsset_read(androidAsset, data.data(), data.size());
				AAsset_close(androidAsset);
				return std::move(data);
			}
		}
		return FileSystemHandlerStandard::ReadBinaryFile(file);
	}

	Result<std::string, FileSystemError> FileSystemHandlerAndroid::ReadTextFile(const Path& file)
	{
		return FileSystemHandlerStandard::ReadTextFile(file);
	}

	bool FileSystemHandlerAndroid::WriteBinaryFile(const Path& file, const ByteBuffer& data)
	{
		return FileSystemHandlerStandard::WriteBinaryFile(file, data);
	}

	bool FileSystemHandlerAndroid::WriteTextFile(const Path& file, const std::string& data)
	{
		return FileSystemHandlerStandard::WriteTextFile(file, data);
	}
}

#endif