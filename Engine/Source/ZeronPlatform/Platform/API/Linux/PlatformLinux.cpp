// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_PLATFORM_LINUX

#	include <Platform/API/Linux/PlatformLinux.h>

#	include <Platform/WindowManager.h>

namespace Zeron
{

	PlatformLinux::PlatformLinux()
		: Platform()
	{}

	PlatformLinux::~PlatformLinux() {}

	bool PlatformLinux::Init()
	{
		const bool result = Platform::Init();
		return result;
	}

	Window* PlatformLinux::CreatePlatformWindow(WindowConfig&& config)
	{
		if (!config.HasValidIcon()) {
			// Load default engine icon
			config.mIcon = _readWindowIcon();
		}
		return mWindowManager->RegisterWindow(config, WindowAPI::GLFW);
	}
}

#endif