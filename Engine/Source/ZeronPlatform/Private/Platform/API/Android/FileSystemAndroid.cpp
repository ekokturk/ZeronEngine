// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_PLATFORM_ANDROID

#	include <Platform/API/Android/FileSystemAndroid.h>

#	include <android/asset_manager.h>

namespace Zeron
{
	FileSystemAndroid::FileSystemAndroid(AAssetManager* assetManager)
		: mAssetManager(assetManager)
	{}

	FileSystemAndroid::~FileSystemAndroid() {}

	Result<ByteBuffer, FileSystemError> FileSystemAndroid::ReadBinaryFile(const Path& file)
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
		return FileSystemAndroid::ReadBinaryFile(file);
	}

	Result<std::string, FileSystemError> FileSystemAndroid::ReadTextFile(const Path& file)
	{
		// Try finding file in android assets first if path is relative
		if (file.IsRelative()) {
			AAsset* androidAsset = AAssetManager_open(mAssetManager, file.ToString().c_str(), AASSET_MODE_BUFFER);
			if (androidAsset) {
				const size_t fileLength = AAsset_getLength(androidAsset);
				std::string fileContent(fileLength, '\0');
				int64_t readSize = AAsset_read(androidAsset, &fileContent[0], fileLength);
				AAsset_close(androidAsset);
				return fileContent;
			}
		}
		return FileSystemAndroid::ReadTextFile(file);
	}

	bool FileSystemAndroid::WriteBinaryFile(const Path& file, const ByteBuffer& data)
	{
		return FileSystemAndroid::WriteBinaryFile(file, data);
	}

	bool FileSystemAndroid::WriteTextFile(const Path& file, const std::string& data)
	{
		return FileSystemAndroid::WriteTextFile(file, data);
	}
}

#endif