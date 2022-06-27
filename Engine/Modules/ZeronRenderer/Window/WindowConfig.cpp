// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Window/WindowConfig.h>

namespace Zeron
{
	WindowConfig::WindowConfig()
		: mName("Zeron Engine")
		, mID(-1)
		, mWidth(800)
		, mHeight(600)
		, mIsFullScreen(false)
	{
	}

	WindowConfig::WindowConfig(const std::string& name, int width, int height, int id)
		: mName(name)
		, mID(id)
		, mWidth(width)
		, mHeight(height)
		, mIsFullScreen(false)
	{
	}
}