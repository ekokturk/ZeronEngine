// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>

namespace Zeron
{
	class Buffer {
	  public:
		Buffer(BufferType type, uint32_t count, uint32_t stride, BufferUsageType usage);
		virtual ~Buffer() = default;

		BufferType GetBufferType() const { return mType; }
		uint32_t GetStride() const { return mStride; }
		uint32_t GetCount() const { return mCount; }
		uint32_t GetSizeInBytes() const { return mStride * mCount; }
		BufferUsageType GetUsageType() const { return mUsage; }

	  private:
		BufferType mType = BufferType::Vertex;
		uint32_t mCount = 0;
		uint32_t mStride = 0;
		BufferUsageType mUsage = BufferUsageType::Default;
	};
}