// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron::Gfx
{
	class FrameBuffer;
	class Window;

	class SwapChain {
	  public:
		SwapChain(const Vec2i& size, uint32_t bufferCount);
		virtual ~SwapChain() = default;

		virtual void Present() = 0;
		virtual void SetVSyncEnabled(bool isEnabled) = 0;
		virtual void SetRefreshRate(uint16_t rate) = 0;

		const Vec2i& GetSize() const;
		uint32_t GetFrameCount() const;
		virtual bool IsVSyncEnabled() const = 0;
		virtual FrameBuffer* GetFrameBuffer() const = 0;

	  protected:
		void _setSize(const Vec2i& size);
		void _setBufferCount(uint32_t count);

	  private:
		Vec2i mSize;
		uint32_t mBufferCount;
	};
}