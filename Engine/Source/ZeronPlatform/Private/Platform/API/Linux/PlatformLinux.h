// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_PLATFORM_LINUX

#	include <Platform/Platform.h>

namespace Zeron
{
	class PlatformLinux : public Platform {
	  public:
		PlatformLinux(const PlatformCreationProps& props);
		~PlatformLinux();

		bool Init() override;
		Window* CreatePlatformWindow(WindowConfig&& config) override;
	};
}

#endif