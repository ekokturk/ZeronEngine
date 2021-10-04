// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron {

	enum class WindowType : uint8_t
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