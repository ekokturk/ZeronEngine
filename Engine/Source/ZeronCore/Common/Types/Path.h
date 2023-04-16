// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	class Path {
	  public:
		Path() = default;
		Path(const Path&) = default;
		Path(Path&&) = default;
		Path(const std::string& str)
			: mPath(str.c_str())
		{}
		Path(const char* str)
			: mPath(str)
		{}

		Path GetParentPath() const { return Path(mPath.parent_path()); }

		Path GetFilename() const { return Path(mPath.filename()); }

		Path GetExtension() const { return Path(mPath.extension()); }

		bool IsRelative() const { return mPath.is_relative(); }

		bool IsEmpty() const { return mPath.empty(); }

		std::string ToString() const { return mPath.string(); }

		Path& operator/=(const std::string& str)
		{
			mPath /= str;
			return *this;
		}

		friend Path operator/(const Path& lhs, const std::string& rhs) { return Path(lhs.mPath / rhs); }

		friend bool operator==(const Path& lhs, const Path& rhs) { return lhs.mPath == rhs.mPath; }

		friend bool operator!=(const Path& lhs, const Path& rhs) { return !(lhs == rhs); }

	  private:
		explicit Path(const std::filesystem::path& path)
			: mPath(path)
		{}
		explicit Path(std::filesystem::path&& path)
			: mPath(std::move(path))
		{}

		std::filesystem::path mPath;
	};
}