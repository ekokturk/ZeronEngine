// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Platform/Platform.h>

#include <Core/SystemEvent.h>
#include <Platform/API/Shared/FileSystemStandard.h>
#include <Platform/SystemEventProcessor.h>
#include <Platform/Window.h>
#include <Platform/WindowManager.h>

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

	std::unique_ptr<Platform> Platform::Create(const PlatformCreationProps& props)
	{
#if ZE_PLATFORM_WIN32
		return std::make_unique<PlatformWin32>(props);
#elif ZE_PLATFORM_LINUX
		return std::make_unique<PlatformLinux>(props);
#elif ZE_PLATFORM_ANDROID
		return std::make_unique<PlatformAndroid>(props);
#else
		return nullptr;
#endif
	}

	Platform::Platform()
	{
		mWindowManager = std::make_unique<WindowManager>([this](const auto& evt, const auto& ctx) {
			_dispatchEvents(evt, ctx);
		});
	}


	Platform::~Platform() {}

	bool Platform::Init()
	{
		// TODO: Move this to implementation
		mFileSystem = std::make_unique<FileSystemStandard>();
		return true;
	}

	void Platform::Update()
	{
		mWindowManager->Update();
	}

	bool Platform::IsExiting() const
	{
		return false;
	}

	FileSystem& Platform::GetFileSystem()
	{
		return *mFileSystem;
	}

	const FileSystem& Platform::GetFileSystem() const
	{
		return *mFileSystem;
	}

	Window* Platform::GetMainWindow() const
	{
		return mWindowManager->GetMainWindow();
	}

	WindowManager* Platform::GetWindowManager() const
	{
		return mWindowManager.get();
	}

	void Platform::_dispatchEvents(const SystemEvent& evt, const SystemEventContext& ctx) {}

	ByteBuffer Platform::_readWindowIcon() const
	{
		ZE_ASSERT(mFileSystem, "Expected file system to be initialized!");
		auto iconResult = mFileSystem->ReadBinaryFile(Path{ "Resources/window_icon.png", PathType::Assets });
		if (!iconResult.HasError()) {
			return iconResult.Value();
		}
		return {};
	}
}