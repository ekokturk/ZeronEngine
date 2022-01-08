// Copyright (C) Eser Kokturk. All Rights Reserved.

#include "WindowConfig.h"

namespace Zeron {

	WindowConfig::WindowConfig()
		: mName("Zeron Engine")
		, mWidth(800)
		, mHeight(600)
		, mID(-1)
		, mIsFullScreen(false)
	{
	}

	WindowConfig::WindowConfig(const std::string& name, int width, int height, int id)
		: mName(name)
		, mWidth(width)
		, mHeight(height)
		, mID(id)
		, mIsFullScreen(false)
	{
	}
}