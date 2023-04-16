// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_PLATFORM_LINUX

#include <Platform/API/Linux/PlatformLinux.h>

#include <Platform/WindowManager.h>

namespace Zeron {

	PlatformLinux::PlatformLinux()
		: Platform()
	{
	}

	PlatformLinux::~PlatformLinux()
	{
	}

	Window* PlatformLinux::CreatePlatformWindow(const WindowConfig& config)
	{
		return mWindowManager->RegisterWindow(config, WindowAPI::GLFW);
	}
}

#endif
