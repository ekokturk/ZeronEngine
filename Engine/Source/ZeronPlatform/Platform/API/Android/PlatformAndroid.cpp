// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_PLATFORM_ANDROID

#include <Platform/API/Android/PlatformAndroid.h>

#include <Platform/WindowManager.h>

namespace Zeron {

	PlatformAndroid::PlatformAndroid(const PlatformConfig& config)
		: Platform(config)
	{
	}

	PlatformAndroid::~PlatformAndroid()
	{
	}

	Window* PlatformAndroid::CreatePlatformWindow(const WindowConfig& config)
	{
		return mWindowManager->RegisterWindow(config, WindowAPI::SDL);
	}
}

#endif
