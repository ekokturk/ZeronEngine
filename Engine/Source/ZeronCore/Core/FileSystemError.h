// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Core/Util.h>

namespace Zeron
{
	enum class FileSystemError
	{
		Unknown = 0,
		NoFileFound,
		IOError,
	};

	namespace Util
	{
		template <>
		constexpr std::string_view ToString(const FileSystemError c)
		{
			switch (c) {
				case FileSystemError::NoFileFound: return "FileSystemError: Unable to find the file";
				case FileSystemError::Unknown:
				default: return "FileSystemError: Unknown error";
			}
		}
	}
}

ZE_ENUM_STRING_FORMAT(Zeron::FileSystemError);