// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/SwapChain.h>

namespace Zeron
{
	SwapChain::SwapChain(const Vec2i& size, uint32_t bufferCount)
		: mSize(size)
		, mBufferCount(bufferCount)
	{
	}

	const Vec2i& SwapChain::GetSize() const
	{
		ZE_ASSERT(mSize != Vec2i::ZERO, "Swapchain cannot have zero size!");
		return mSize;
	}

	uint32_t SwapChain::GetFrameCount() const
	{
		return mBufferCount;
	}

	void SwapChain::_setSize(const Vec2i& size)
	{
		ZE_ASSERT(mSize != Vec2i::ZERO, "Swapchain cannot have zero size!");
		mSize = size;
	}
}
