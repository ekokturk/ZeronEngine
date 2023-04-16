// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Platform/CommandLineArgs.h>
#include <Platform/FileSystem.h>

#if ZE_PLATFORM_WIN32
#	include <Platform/API/Win32/PlatformWin32.h>
using EntryPlatform = ::Zeron::PlatformWin32;
#elif ZE_PLATFORM_LINUX
#	include <Platform/API/Linux/PlatformLinux.h>
using EntryPlatform = ::Zeron::PlatformLinux;
#elif ZE_PLATFORM_ANDROID
#	include <game-activity/native_app_glue/android_native_app_glue.h>
#	include <Platform/API/Android/PlatformAndroid.h>
using EntryPlatform = ::Zeron::PlatformAndroid;
#endif

namespace Zeron
{
	using EntryPointFn = bool (*)(Platform&, const CommandLineArgs&);

	struct EntryPoint {
		template <typename... PlatformArgs>
		static void Main(Platform& platform, const CommandLineArgs& cmdArgs, EntryPointFn fn)
		{
			if (platform.Init()) {
				// Globals
				FileSystem::Init(&platform.GetFileSystem());

				// App Entry
				fn(platform, cmdArgs);
			}
		}
	};
}

#define ZERON_WIN32_ENTRY_POINT(fn)                        \
	int main(int argc, char** argv)                        \
	{                                                      \
		::Zeron::CommandLineArgs cmdArgs;                  \
		cmdArgs.Process(argc, argv);                       \
		auto platform = std::make_unique<EntryPlatform>(); \
		::Zeron::EntryPoint::Main(*platform, cmdArgs, fn); \
	}

#define ZERON_LINUX_ENTRY_POINT(fn)                        \
	int main(int argc, char** argv)                        \
	{                                                      \
		::Zeron::CommandLineArgs cmdArgs;                  \
		cmdArgs.Process(argc, argv);                       \
		auto platform = std::make_unique<EntryPlatform>(); \
		::Zeron::EntryPoint::Main(*platform, cmdArgs, fn); \
	}

#define ZERON_ANDROID_ENTRY_POINT(fn)                                         \
	void android_main(android_app* app)                                       \
	{                                                                         \
		auto platform = std::make_unique<EntryPlatform>(app);                 \
		::Zeron::EntryPoint::Main(*platform, ::Zeron::CommandLineArgs{}, fn); \
	}

#if ZE_PLATFORM_WIN32
#	define ZERON_DECLARE_ENTRY_POINT(fn) ZERON_WIN32_ENTRY_POINT(fn)
#elif ZE_PLATFORM_LINUX
#	define ZERON_DECLARE_ENTRY_POINT(fn) ZERON_LINUX_ENTRY_POINT(fn)
#elif ZE_PLATFORM_ANDROID
#	define ZERON_DECLARE_ENTRY_POINT(fn) ZERON_ANDROID_ENTRY_POINT(fn)
#else
#	define ZERON_DECLARE_ENTRY_POINT(fn)
#endif