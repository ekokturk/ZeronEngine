// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_PLATFORM_ANDROID

#	include <Platform/API/Android/WindowAndroid.h>

#	include <android/native_window.h>

namespace Zeron
{

	WindowAndroid::WindowAndroid(const WindowConfig& config, ANativeWindow* nativeWindow)
		: Window(config, WindowAPI::Android)
		, mNativeWindow(nativeWindow)
	{}

	WindowAndroid::~WindowAndroid() {}

	void WindowAndroid::SetVisible() {}

	void WindowAndroid::SetHidden() {}

	void WindowAndroid::SetName(const std::string& name) {}

	void WindowAndroid::SetAspectRatio(int numerator, int denominator) {}

	void WindowAndroid::SetSize(int width, int height) {}

	void WindowAndroid::SetSizeLimits(int minWidth, int maxWidth, int minHeight, int maxHeight) {}

	void WindowAndroid::SetScreenPosition(int posX, int posY) {}

	void WindowAndroid::SetClipCursor(bool shouldClip) {}

	void* WindowAndroid::GetApiHandle() const
	{
		return mNativeWindow;
	}

	SystemHandle WindowAndroid::GetSystemHandle() const
	{
		return SystemHandle{ mNativeWindow };
	}

	void WindowAndroid::SetMinimized() {}

	void WindowAndroid::SetMaximized() {}

	void WindowAndroid::SetRestored() {}

	void WindowAndroid::SetFocused() {}

	void WindowAndroid::SetAttention() {}
}

#endif