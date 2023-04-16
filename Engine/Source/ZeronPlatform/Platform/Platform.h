// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Platform/SystemEventProcessor.h>

namespace Zeron
{
	class IFileSystemHandler;
	class SystemEvent;
	class SystemEventProcessor;
	struct WindowConfig;
	class WindowManager;
	class Window;

	class Platform {
	public:
		virtual ~Platform();

		virtual bool Init();
		virtual void Update();
		bool IsExiting() const;

		// Filesystem
		IFileSystemHandler& GetFileSystem();
		const IFileSystemHandler& GetFileSystem() const;

		// Window
		virtual Window* GetMainWindow() const;
		virtual Window* CreatePlatformWindow(const WindowConfig& config) = 0;
		WindowManager* GetWindowManager() const;

	protected:
		Platform();

		void _dispatchEvents(const SystemEvent& evt, const SystemEvent::Context& ctx);

		std::unique_ptr<WindowManager> mWindowManager;
		std::unique_ptr<IFileSystemHandler> mFileSystem;
	};
}
