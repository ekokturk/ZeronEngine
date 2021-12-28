// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "GraphicsTypes.h"

namespace Zeron {

	class Buffer {
	public:
		virtual ~Buffer() = default;

		uint32_t GetStride() const { return mStride; }
		uint32_t GetSize() const { return mSize; }

		virtual BufferType GetBufferType() const = 0;
	
	protected:
		uint32_t mSize = 0;
		uint32_t mStride = 0;
	};
}
