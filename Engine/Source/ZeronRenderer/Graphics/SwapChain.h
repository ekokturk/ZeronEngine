// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	class RenderTarget;
	class Window;
	
	class SwapChain {
	public:
		SwapChain(Window& window);
		virtual ~SwapChain() = default;

		virtual void SwapBuffers() = 0;
		virtual void SetViewport(const Vec2i & pos, const Vec2i & size) = 0;
		virtual void SetVSyncEnabled(bool isEnabled) = 0;
		virtual void SetRefreshRate(uint16_t rate) = 0;

		Vec2i GetWindowSize() const;
		virtual bool IsVSyncEnabled() const = 0;
		virtual RenderTarget* GetRenderTarget() const = 0;
		
	private:
		Window* mWindow;
	};
}
