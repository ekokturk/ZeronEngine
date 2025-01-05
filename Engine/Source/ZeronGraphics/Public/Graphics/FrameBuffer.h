// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron::Gfx
{
	class FrameBuffer {
	  public:
		virtual ~FrameBuffer() = default;

		const Vec2i& GetExtent() const { return mExtent; }

	  protected:
		FrameBuffer(const Vec2i& extent)
			: mExtent(extent)
		{}

		Vec2i mExtent;
	};
}