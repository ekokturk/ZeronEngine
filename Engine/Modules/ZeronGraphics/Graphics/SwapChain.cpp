// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/SwapChain.h>

namespace Zeron
{
	SwapChain::SwapChain(const Vec2i& size)
		: mSize(size)
	{
	}

	const Vec2i& SwapChain::GetSize() const
	{
		ZE_ASSERT(mSize != Vec2i::ZERO, "Swapchain cannot have zero size!");
		return mSize;
	}

	void SwapChain::SetSize_(const Vec2i& size)
	{
		ZE_ASSERT(mSize != Vec2i::ZERO, "Swapchain cannot have zero size!");
		mSize = size;
	}
}
