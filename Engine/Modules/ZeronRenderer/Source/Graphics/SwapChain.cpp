// Copyright (C) Eser Kokturk. All Rights Reserved.

#include "SwapChain.h"
#include "Window/Window.h"

namespace Zeron
{
	SwapChain::SwapChain(Window& window)
		: mWindow(&window) {
	}

	Vec2i SwapChain::GetWindowSize() const
	{
		ZE_ASSERT(mWindow, "Swapchain window is null!");
		return mWindow->GetSize();
	}
}
