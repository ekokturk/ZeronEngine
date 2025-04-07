// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Core/FileSystem.h>
#include <Platform/CommandLineArgs.h>
#include <Platform/Platform.h>

namespace Zeron
{
	using EntryPointFn = bool (*)(Platform&, const CommandLineArgs&);

	struct EntryPoint {
		template <typename... PlatformArgs>
		static void Main(Platform& platform, const CommandLineArgs& cmdArgs, EntryPointFn fn)
		{
			if (platform.Init()) {
				// Globals
				Locator::Register<FileSystem>(&platform.GetFileSystem());

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
		auto platform = ::Zeron::Platform::Create();       \
		::Zeron::EntryPoint::Main(*platform, cmdArgs, fn); \
	}

#define ZERON_LINUX_ENTRY_POINT(fn)                        \
	int main(int argc, char** argv)                        \
	{                                                      \
		::Zeron::CommandLineArgs cmdArgs;                  \
		cmdArgs.Process(argc, argv);                       \
		auto platform = ::Zeron::Platform::Create();       \
		::Zeron::EntryPoint::Main(*platform, cmdArgs, fn); \
	}

#define ZERON_ANDROID_ENTRY_POINT(fn)                                         \
	void android_main(struct android_app* app)                                \
	{                                                                         \
		::Zeron::PlatformCreationProps props;                                 \
		props.mAppHandle = app;                                               \
		auto platform = ::Zeron::Platform::Create(props);                     \
		::Zeron::EntryPoint::Main(*platform, ::Zeron::CommandLineArgs{}, fn); \
	}

#if ZE_PLATFORM_WIN32
#	define ZERON_DECLARE_ENTRY_POINT(fn) ZERON_WIN32_ENTRY_POINT(fn)
#elif ZE_PLATFORM_LINUX
#	define ZERON_DECLARE_ENTRY_POINT(fn) ZERON_LINUX_ENTRY_POINT(fn)
#elif ZE_PLATFORM_ANDROID
#	include <game-activity/native_app_glue/android_native_app_glue.h>
#	define ZERON_DECLARE_ENTRY_POINT(fn) ZERON_ANDROID_ENTRY_POINT(fn)
#else
#	define ZERON_DECLARE_ENTRY_POINT(fn)
#endif