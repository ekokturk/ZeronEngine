// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "GraphicsTypes.h"

namespace Zeron {

	class Buffer {
	public:
		virtual ~Buffer() = default;

		[[nodiscard]] uint32_t GetStride() const { return mStride; }
		[[nodiscard]] uint32_t GetCount() const { return mCount; }

		[[nodiscard]] virtual BufferType GetBufferType() const = 0;
	
	protected:
		uint32_t mCount = 0;
		uint32_t mStride = 0;
	};
}
