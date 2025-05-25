// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	enum class PathType
	{
		Raw,
		WorkingDir,
		Assets,
		UserFiles,
		Cache,
		Temp
	};

	class Path {
	  public:
		Path() = default;
		Path(const Path&) = default;
		Path(Path&&) = default;

		Path(const std::string& str, PathType type = PathType::WorkingDir)
			: mPath(str)
			, mType(type)
		{}

		Path(const char* str, PathType type = PathType::WorkingDir)
			: mPath(str)
			, mType(type)
		{}

		Path GetParentPath() const { return Path(mPath.parent_path(), mType); }

		std::string GetFilename() const { return mPath.filename().string(); }

		std::string GetExtension() const { return mPath.extension().string(); }

		bool IsRelative() const { return mPath.is_relative(); }

		bool IsEmpty() const { return mPath.empty(); }

		std::string ToString() const { return mPath.string(); }

		PathType GetType() const { return mType; }

		void SetType(PathType type) { mType = type; }

		Path& operator/=(const std::string& str)
		{
			mPath /= str;
			return *this;
		}

		Path& operator/=(const Path& path)
		{
			mPath = mPath / path.mPath;
			return *this;
		}

		friend Path operator/(const Path& lhs, const std::string& rhs) { return Path(lhs.mPath / rhs, lhs.mType); }

		friend bool operator==(const Path& lhs, const Path& rhs) { return lhs.mPath == rhs.mPath && lhs.mType == rhs.mType; }

		friend bool operator!=(const Path& lhs, const Path& rhs) { return !(lhs == rhs); }

	  private:
		explicit Path(const std::filesystem::path& path, PathType type = PathType::WorkingDir)
			: mPath(path)
			, mType(type)
		{}

		explicit Path(std::filesystem::path&& path, PathType type = PathType::WorkingDir)
			: mPath(std::move(path))
			, mType(type)
		{}

		std::filesystem::path mPath;
		PathType mType = PathType::WorkingDir;
	};
}