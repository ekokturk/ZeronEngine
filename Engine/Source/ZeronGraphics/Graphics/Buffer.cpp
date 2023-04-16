// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/Buffer.h>

namespace Zeron
{
	Buffer::Buffer(BufferType type, uint32_t count, uint32_t stride, BufferUsageType usage)
		: mType(type)
		, mCount(count)
		, mStride(stride)
		, mUsage(usage)
	{}
}