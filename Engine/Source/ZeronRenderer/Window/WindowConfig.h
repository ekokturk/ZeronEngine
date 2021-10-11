// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron {

	enum class FullScreenType : uint8_t
	{
		Borderless,		// Borderless Fullscreen Window
		Monitor			// Display is adjusted to window
	};

	enum class WindowAPI : uint8_t
	{
		Null,
		GLFW,
		SDL,
		Win32
	};

	struct WindowConfig
	{
		WindowConfig();
		WindowConfig(const std::string& name, int width, int height, int id);

		std::string mName;
		int mID;
		int mWidth;
		int mHeight;
		bool mIsFullScreen;
	};

}