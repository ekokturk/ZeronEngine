// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_PLATFORM_WIN32

#include <Platform/Platform.h>

#include <Input/KeyCode.h>
#include <Input/MouseCode.h>

namespace Zeron
{
	class PlatformWin32 : public Platform
	{
	public:
		PlatformWin32(const PlatformConfig& config);
		~PlatformWin32();

		Window* CreatePlatformWindow(const WindowConfig& config) override;

		static KeyCode GetKeyCode(WPARAM wParam, LPARAM lParam);
		static MouseCode GetMouseCode(WPARAM wParam, LPARAM lParam);
	};
}

#endif