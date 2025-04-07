// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Platform/API/Shared/FileSystemStandard.h>

namespace Zeron
{
	Result<ByteBuffer, FileSystemError> FileSystemStandard::ReadBinaryFile(const Path& file)
	{
		std::fstream fs;
		fs.open(file.ToString(), std::ios::in | std::ios::binary | std::ios::ate);
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
		std::fstream fs;
		fs.open(file.ToString(), std::ios::in);
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
		std::fstream fs;
		fs.open(file.ToString(), std::ios::out | std::ios::binary | std::ios::trunc);
		if (fs.is_open() && !data.empty()) {
			fs.write(reinterpret_cast<const char*>(data.data()), data.size());
			fs.close();
			return true;
		}
		return false;
	}

	bool FileSystemStandard::WriteTextFile(const Path& file, const std::string& data)
	{
		std::fstream fs;
		fs.open(file.ToString(), std::ios::out | std::ios::trunc);
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
		if (!std::filesystem::create_directories(dir.ToString(), ec)) {
			if (ec) {
				return { FileSystemError::IOError };
			}
		}
		return {};
	}

	Result<void, FileSystemError> FileSystemStandard::RemoveFile(const Path& file)
	{
		std::error_code ec;
		if (!FileExists(file) || !std::filesystem::remove(file.ToString(), ec)) {
			return { FileSystemError::NoFileFound };
		}
		return {};
	}

	Result<void, FileSystemError> FileSystemStandard::RemoveDir(const Path& dir, bool recursive)
	{
		std::error_code ec;
		if (recursive) {
			std::filesystem::remove_all(dir.ToString(), ec);
		}
		else {
			std::filesystem::remove(dir.ToString(), ec);
		}

		if (ec) {
			return { FileSystemError::IOError };
		}

		return {};
	}

	bool FileSystemStandard::FileExists(const Path& file) const
	{
		return std::filesystem::is_regular_file(file.ToString());
	}

	bool FileSystemStandard::DirExists(const Path& dir) const
	{
		return std::filesystem::is_directory(dir.ToString());
	}
}