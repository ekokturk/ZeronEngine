// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Platform/API/Shared/FileSystemHandlerStandard.h>

namespace Zeron
{
	Result<ByteBuffer, FileSystemError> FileSystemHandlerStandard::ReadBinaryFile(const Path& file)
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
		return {FileSystemError::NoFileFound};
	}

	Result<std::string, FileSystemError> FileSystemHandlerStandard::ReadTextFile(const Path& file)
	{
		std::fstream fs;
		fs.open(file.ToString(), std::ios::in);
		if (fs.is_open()) {
			fs.seekg(0, std::ios::beg);
			auto data = std::string(std::istreambuf_iterator<char>(fs), std::istreambuf_iterator<char>());
			fs.close();
			return std::move(data);
		}
		return {FileSystemError::NoFileFound};
	}

	bool FileSystemHandlerStandard::WriteBinaryFile(const Path& file, const ByteBuffer& data)
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

	bool FileSystemHandlerStandard::WriteTextFile(const Path& file, const std::string& data)
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
}
