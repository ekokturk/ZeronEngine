// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	class RenderTarget;
	class Window;
	
	class SwapChain {
	public:
		SwapChain(const Vec2i& size);
		virtual ~SwapChain() = default;

		virtual void SwapBuffers() = 0;
		virtual void SetVSyncEnabled(bool isEnabled) = 0;
		virtual void SetRefreshRate(uint16_t rate) = 0;

		const Vec2i& GetSize() const;
		virtual bool IsVSyncEnabled() const = 0;
		virtual RenderTarget* GetRenderTarget() const = 0;

	protected:
		void SetSize_(const Vec2i& size);
	
	private:
		Vec2i mSize;
	};
}
