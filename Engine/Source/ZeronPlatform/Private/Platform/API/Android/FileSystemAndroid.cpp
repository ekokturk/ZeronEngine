// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_PLATFORM_ANDROID

#	include <Platform/API/Android/FileSystemAndroid.h>

#	include <android/asset_manager.h>

namespace Zeron
{
	FileSystemAndroid::FileSystemAndroid(AAssetManager* assetManager, const char* internalDataPath, const char* externalDataPath)
		: mAssetManager(assetManager)
		, mInternalDataPath(internalDataPath)
		, mExternalDataPath(externalDataPath)
	{}

	FileSystemAndroid::~FileSystemAndroid() {}

	Result<ByteBuffer, FileSystemError> FileSystemAndroid::ReadBinaryFile(const Path& file)
	{
		if (file.GetType() == PathType::Assets) {
			AAsset* androidAsset = AAssetManager_open(mAssetManager, file.ToString().c_str(), AASSET_MODE_BUFFER);
			if (androidAsset) {
				const size_t fileLength = AAsset_getLength(androidAsset);
				ByteBuffer data(fileLength);
				int64_t readSize = AAsset_read(androidAsset, data.data(), data.size());
				AAsset_close(androidAsset);
				return std::move(data);
			}
		}
		return FileSystemStandard::ReadBinaryFile(file);
	}

	Result<std::string, FileSystemError> FileSystemAndroid::ReadTextFile(const Path& file)
	{
		if (file.GetType() == PathType::Assets) {
			AAsset* androidAsset = AAssetManager_open(mAssetManager, file.ToString().c_str(), AASSET_MODE_BUFFER);
			if (androidAsset) {
				const size_t fileLength = AAsset_getLength(androidAsset);
				std::string fileContent(fileLength, '\0');
				int64_t readSize = AAsset_read(androidAsset, &fileContent[0], fileLength);
				AAsset_close(androidAsset);
				return fileContent;
			}
		}
		return FileSystemStandard::ReadTextFile(file);
	}

	bool FileSystemAndroid::WriteBinaryFile(const Path& file, const ByteBuffer& data)
	{
		return FileSystemStandard::WriteBinaryFile(file, data);
	}

	bool FileSystemAndroid::WriteTextFile(const Path& file, const std::string& data)
	{
		return FileSystemStandard::WriteTextFile(file, data);
	}

	std::string FileSystemAndroid::ResolvePath(const Path& path) const
	{
		switch (path.GetType()) {
			case PathType::WorkingDir: return (Path(mInternalDataPath) / path.ToString()).ToString();
			case PathType::Cache: return (Path(mInternalDataPath) / "cache" / path.ToString()).ToString();
			case PathType::UserFiles: return mExternalDataPath;
		}
		return FileSystemStandard::ResolvePath(path);
	}
}

#endif