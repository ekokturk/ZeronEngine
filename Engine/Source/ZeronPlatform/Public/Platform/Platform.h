// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Platform/SystemEventProcessor.h>

namespace Zeron
{
	class FileSystem;
	class SystemEvent;
	class SystemEventProcessor;
	struct WindowConfig;
	class WindowManager;
	class Window;

	struct PlatformCreationProps {
		void* mAppHandle = nullptr;
	};

	class Platform {
	  public:
		static std::unique_ptr<Platform> Create(const PlatformCreationProps& props = {});

		virtual ~Platform();

		virtual bool Init();
		virtual void Update();
		bool IsExiting() const;

		// Filesystem
		FileSystem& GetFileSystem();
		const FileSystem& GetFileSystem() const;

		// Window
		virtual Window* GetMainWindow() const;
		virtual Window* CreatePlatformWindow(WindowConfig&& config) = 0;
		WindowManager* GetWindowManager() const;

	  protected:
		Platform();

		void _dispatchEvents(const SystemEvent& evt, const SystemEventContext& ctx);

		ByteBuffer _readWindowIcon() const;

		std::unique_ptr<WindowManager> mWindowManager;
		std::unique_ptr<FileSystem> mFileSystem;
	};
}