// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Platform/WindowManager.h>

#include <Platform/Window.h>

#include <Platform/API/Shared/GLFW/SystemEventProcessorGLFW.h>
#include <Platform/API/Shared/GLFW/WindowGLFW.h>
#include <Platform/API/Win32/SystemEventProcessorWin32.h>
#include <Platform/API/Win32/WindowWin32.h>

namespace Zeron
{
	WindowManager::WindowManager(SystemEventProcessor::DispatchFn onSystemEvent)
		: mExternalDispatchCallback(std::move(onSystemEvent))
		, mMainWindow(nullptr)
		, mFocusedWindow(nullptr)
		, mHoveredWindow(nullptr)
	{
		mWindowList.reserve(8);
	}

	WindowManager::~WindowManager()
	{
		_removePendingWindows();
	}

	Window* WindowManager::RegisterWindow(const WindowConfig& config, WindowAPI defaultType)
	{
		ZE_ASSERT(!(defaultType == WindowAPI::Null && config.mPreferredAPI == WindowAPI::Null), "Window should have a default API type");
		const WindowAPI windowType = config.mPreferredAPI == WindowAPI::Null ? defaultType : config.mPreferredAPI;

		std::unique_ptr<Window> window = nullptr;
		auto dispatchCallback = [this](const SystemEvent& evt, const SystemEvent::Context& context) {
			if (context.mWindow) {
				context.mWindow->OnSystemEvent(evt);
				if (std::get_if<SystemEvent::WindowClosed>(&evt.GetData())) {
					mWindowsToRemove.emplace(context.mWindow);
				}
			}
			mExternalDispatchCallback(evt, context);
		};
		auto contextCallback = [this](SystemHandle handle) {
			return SystemEvent::Context{ GetWindowByHandle(handle) };
		};
		switch (windowType) {
#if ZE_WINDOW_WIN32
			case WindowAPI::Win32: {
				SystemEventProcessor* proc = mEventProcessorList.Add<SystemEventProcessorWin32>(
					static_cast<size_t>(WindowAPI::Win32), std::move(dispatchCallback), std::move(contextCallback)
				);
				window = std::make_unique<WindowWin32>(config, &SystemEventProcessorWin32::WndProcRouter, proc);
			} break;
#endif
#if ZE_WINDOW_GLFW
			case WindowAPI::GLFW: {
				SystemEventProcessor* proc = mEventProcessorList.Add<SystemEventProcessorGLFW>(
					static_cast<size_t>(WindowAPI::GLFW), std::move(dispatchCallback), std::move(contextCallback)
				);
				window = std::make_unique<WindowGLFW>(config, proc);
				static_cast<SystemEventProcessorGLFW*>(proc)->registerEventCallbacks(static_cast<WindowGLFW*>(window.get()));
			} break;
#endif
			case WindowAPI::Null:
			default: ZE_FAIL("Window API is not supported by this platform");
		}

		mWindowList.emplace_back(std::move(window));
		Window* newWindow = mWindowList.back().get();
		if (!mMainWindow) {
			mMainWindow = newWindow;
		}

		return newWindow;
	}

	void WindowManager::Update()
	{
		for (auto& window : mWindowList) {
			window->FlushEvents();
		}
		_removePendingWindows();

		mEventProcessorList.ProcessEvents();

		for (auto& window : mWindowList) {
			window->Update();
		}
	}

	void WindowManager::ForEachWindow(std::function<void(Window&)> callback)
	{
		for (auto& window : mWindowList) {
			callback(*window);
		}
	}

	Window* WindowManager::GetMainWindow() const
	{
		return mMainWindow;
	}

	Window* WindowManager::GetWindowByHandle(const SystemHandle& handle)
	{
		auto found = std::find_if(mWindowList.begin(), mWindowList.end(), [&handle](const std::unique_ptr<Window>& window) {
			return window && window->GetSystemHandle() == handle;
		});
		return found != mWindowList.end() ? found->get() : nullptr;
	}

	uint32_t WindowManager::GetWindowCount(WindowAPI type) const
	{
		uint32_t count = 0;
		for (const std::unique_ptr<Window>& window : mWindowList) {
			if (window->GetWindowType() == type) {
				++count;
			}
		}
		return count;
	}

	void WindowManager::_removePendingWindows()
	{
		if (!mWindowsToRemove.empty()) {
			for (const Window* windowToRemove : mWindowsToRemove) {
				const size_t processorId = static_cast<size_t>(windowToRemove->GetWindowType());
				if (GetWindowCount(windowToRemove->GetWindowType()) == 1) {
					mProcessorsToRemove.push_back(processorId);
				}
			}

			mWindowList.erase(
				std::remove_if(
					mWindowList.begin(),
					mWindowList.end(),
					[this](const std::unique_ptr<Window>& ownedWindow) {
						return mWindowsToRemove.count(ownedWindow.get()) > 0;
					}
				),
				mWindowList.end()
			);
			mWindowsToRemove = {};
		}

		if (!mProcessorsToRemove.empty()) {
			for (const size_t id : mProcessorsToRemove) {
				mEventProcessorList.Remove(id);
			}
			mProcessorsToRemove = {};
		}
	}
}