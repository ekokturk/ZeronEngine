// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>

namespace Zeron {

	class Buffer {
	public:
		virtual ~Buffer() = default;

		uint32_t GetStride() const { return mStride; }
		uint32_t GetCount() const { return mCount; }

		virtual BufferType GetBufferType() const = 0;
	
	protected:
		uint32_t mCount = 0;
		uint32_t mStride = 0;
	};
}
