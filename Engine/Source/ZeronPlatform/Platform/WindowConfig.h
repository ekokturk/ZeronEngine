// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	enum class FullScreenType : uint8_t
	{
		Borderless,	 // Borderless Fullscreen Window
		Monitor		 // Display is adjusted to window
	};

	enum class WindowAPI : uint8_t
	{
		Null,
		GLFW,
		Win32,
		Android
	};

	struct WindowConfig {
		std::string mName = "Zeron Engine";
		int mWidth = 800;
		int mHeight = 600;
		int mID = -1;
		bool mIsFullScreen = false;
		WindowAPI mPreferredAPI = WindowAPI::Null;
	};
}