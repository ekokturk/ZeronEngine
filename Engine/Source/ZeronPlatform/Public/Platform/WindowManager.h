// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Platform/SystemEventProcessor.h>
#include <Platform/WindowConfig.h>

namespace Zeron
{
	class Window;

	class WindowManager {
	  public:
		WindowManager(SystemEventProcessor::DispatchFn onSystemEvent);
		~WindowManager();

		void Update();

		Window* RegisterWindow(const WindowConfig& config, WindowAPI defaultType = WindowAPI::Null);

		void ForEachWindow(std::function<void(Window&)> callback);

		Window* GetMainWindow() const;
		Window* GetWindowByHandle(const SystemHandle& handle);
		uint32_t GetWindowCount(WindowAPI type) const;

	  private:
		void _removePendingWindows();

		UniqueSystemEventProcessorList mEventProcessorList;
		std::vector<std::unique_ptr<Window>> mWindowList;

		std::vector<size_t> mProcessorsToRemove;
		std::set<Window*> mWindowsToRemove;

		SystemEventProcessor::DispatchFn mExternalDispatchCallback;

		Window* mMainWindow;
		Window* mFocusedWindow;
		Window* mHoveredWindow;
	};
}