// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Platform/API/Shared/FileSystemStandard.h>

namespace Zeron
{
	Result<ByteBuffer, FileSystemError> FileSystemStandard::ReadBinaryFile(const Path& file)
	{
		const std::string path = ResolvePath(file);
		std::fstream fs;
		fs.open(path, std::ios::in | std::ios::binary | std::ios::ate);
		if (fs.is_open()) {
			const std::streamsize fileSize = fs.tellg();
			fs.seekg(0, std::ios::beg);
			ByteBuffer data(fileSize);
			fs.read(reinterpret_cast<char*>(data.data()), fileSize);
			fs.close();
			return std::move(data);
		}
		return { FileSystemError::NoFileFound };
	}

	Result<std::string, FileSystemError> FileSystemStandard::ReadTextFile(const Path& file)
	{
		const std::string path = ResolvePath(file);
		std::fstream fs;
		fs.open(path, std::ios::in);
		if (fs.is_open()) {
			fs.seekg(0, std::ios::beg);
			auto data = std::string(std::istreambuf_iterator<char>(fs), std::istreambuf_iterator<char>());
			fs.close();
			return std::move(data);
		}
		return { FileSystemError::NoFileFound };
	}

	bool FileSystemStandard::WriteBinaryFile(const Path& file, const ByteBuffer& data)
	{
		const std::string path = ResolvePath(file);
		std::fstream fs;
		fs.open(path, std::ios::out | std::ios::binary | std::ios::trunc);
		if (fs.is_open() && !data.empty()) {
			fs.write(reinterpret_cast<const char*>(data.data()), data.size());
			fs.close();
			return true;
		}
		return false;
	}

	bool FileSystemStandard::WriteTextFile(const Path& file, const std::string& data)
	{
		const std::string path = ResolvePath(file);
		std::fstream fs;
		fs.open(path, std::ios::out | std::ios::trunc);
		if (fs.is_open() && !data.empty()) {
			fs.write(data.data(), data.size());
			fs.close();
			return true;
		}
		return false;
	}

	Result<void, FileSystemError> FileSystemStandard::CreateDir(const Path& dir)
	{
		std::error_code ec;
		const std::string path = ResolvePath(dir);
		if (!std::filesystem::create_directories(path, ec)) {
			if (ec) {
				return { FileSystemError::IOError };
			}
		}
		return {};
	}

	Result<void, FileSystemError> FileSystemStandard::RemoveFile(const Path& file)
	{
		std::error_code ec;
		const std::string path = ResolvePath(file);
		if (!FileExists(file) || !std::filesystem::remove(path, ec)) {
			return { FileSystemError::NoFileFound };
		}
		return {};
	}

	Result<void, FileSystemError> FileSystemStandard::RemoveDir(const Path& dir, bool recursive)
	{
		std::error_code ec;
		const std::string path = ResolvePath(dir);
		if (recursive) {
			std::filesystem::remove_all(path, ec);
		}
		else {
			std::filesystem::remove(path, ec);
		}

		if (ec) {
			return { FileSystemError::IOError };
		}

		return {};
	}

	bool FileSystemStandard::FileExists(const Path& file) const
	{
		const std::string path = ResolvePath(file);
		return std::filesystem::is_regular_file(path);
	}

	bool FileSystemStandard::DirExists(const Path& dir) const
	{
		const std::string path = ResolvePath(dir);
		return std::filesystem::is_directory(path);
	}

	std::string FileSystemStandard::ResolvePath(const Path& path) const
	{
		switch (path.GetType()) {
			case PathType::Raw: return path.ToString();
			case PathType::WorkingDir: return (std::filesystem::current_path() / path.ToString()).string();
			case PathType::Temp: return (std::filesystem::temp_directory_path() / path.ToString()).string();
			case PathType::Assets: return path.IsRelative() ? path.ToString() : "";
			case PathType::Cache:
			case PathType::UserFiles: {
				ZE_FAIL("Path resolution is not implemented");
			}
		}
		return "";
	}
}