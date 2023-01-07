// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Platform/SystemEventProcessor.h>

namespace Zeron
{
	class SystemEvent;
	class SystemEventProcessor;
	struct WindowConfig;
	class WindowManager;
	class Window;

	struct PlatformConfig {
	};

	class Platform
	{
	public:
		static std::unique_ptr<Platform> CreatePlatformInstance(const PlatformConfig& config = {});

		virtual ~Platform();

		void Update();

		virtual Window* CreatePlatformWindow(const WindowConfig& config) = 0;

		bool IsExiting() const;
		WindowManager* GetWindowManager() const;

	protected:
		Platform(const PlatformConfig& config = {});

		void _dispatchEvents(const SystemEvent& evt, const SystemEvent::Context& ctx);

		const PlatformConfig mConfig;
		std::unique_ptr<WindowManager> mWindowManager;

	};
}
