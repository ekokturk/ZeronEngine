// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Platform/Platform.h>

#include <Platform/SystemEvent.h>
#include <Platform/SystemEventProcessor.h>
#include <Platform/Window.h>
#include <Platform/WindowManager.h>

#include <Platform/API/Win32/PlatformWin32.h>
#include <Platform/API/Linux/PlatformLinux.h>
#include <Platform/API/Android/PlatformAndroid.h>

namespace Zeron {

	std::unique_ptr<Platform> Platform::CreatePlatformInstance(const PlatformConfig& config)
	{
#if ZE_PLATFORM_WIN32
		return std::make_unique<PlatformWin32>(config);
#elif ZE_PLATFORM_LINUX
		return std::make_unique<PlatformLinux>(config);
#elif ZE_PLATFORM_ANDROID
		return std::make_unique<PlatformAndroid>(config);
#else
		ZE_FAIL("Platform not implemented");
		return nullptr;
#endif
	}

	Platform::Platform(const PlatformConfig& config)
		: mConfig(config)
	{
		mWindowManager = std::make_unique<WindowManager>([this](const auto& evt, const auto& ctx) {
			_dispatchEvents(evt, ctx);
		});
	}

	Platform::~Platform()
	{
	}

	void Platform::_dispatchEvents(const SystemEvent& evt, const SystemEvent::Context& ctx)
	{
	}

	void Platform::Update()
	{
		mWindowManager->Update();

	}

	bool Platform::IsExiting() const
	{
		return false;
	}

	WindowManager* Platform::GetWindowManager() const
	{
		return mWindowManager.get();
	}
}
