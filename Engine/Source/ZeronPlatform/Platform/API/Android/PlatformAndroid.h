// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_PLATFORM_ANDROID

#include <Platform/Platform.h>

namespace Zeron
{
	class PlatformAndroid : public Platform
	{
	public:
		PlatformAndroid(const PlatformConfig& config);
		~PlatformAndroid();

		Window* CreatePlatformWindow(const WindowConfig& config) override;
	};
}

#endif