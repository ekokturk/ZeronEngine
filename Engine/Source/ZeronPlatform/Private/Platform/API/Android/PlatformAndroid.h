// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_PLATFORM_ANDROID

#	include <game-activity/native_app_glue/android_native_app_glue.h>

#	include <Platform/Platform.h>

struct android_app;

namespace Zeron
{
	class WindowAndroid;

	class PlatformAndroid : public Platform {
	  public:
		PlatformAndroid(const PlatformCreationProps& props);
		~PlatformAndroid();

		// Window API
		bool Init() override;
		void Update() override;
		Window* GetMainWindow() const override;
		Window* CreatePlatformWindow(WindowConfig&& config) override;

		// Android API
		void onAppCmd(int cmd);
		static KeyCode GetKeyCode(int keyCode);

	  private:
		void _processEvents();
		void _handleInputEvents();
		void _initDirectories();

		android_app* mApp;
		std::unique_ptr<WindowAndroid> mWindow;
		// TODO: Change these to a state enum
		bool mDestroyRequested;
	};
}

#endif