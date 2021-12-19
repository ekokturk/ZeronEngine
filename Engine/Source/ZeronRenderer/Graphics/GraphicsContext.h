// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	class Window;
	
	class GraphicsContext {
	public:
		GraphicsContext(Window* window);
		virtual ~GraphicsContext() = default;

		virtual void SetRenderTarget() = 0;
		virtual void ClearBuffer(Color color = Color::Black) = 0;
		virtual void SwapBuffers() = 0;
		virtual void SetViewport(const Vec2i& pos, const Vec2i& size) = 0;
		virtual void SetVSyncEnabled(bool isEnabled) = 0;
		virtual void SetRefreshRate(uint16_t rate) = 0;

		virtual bool IsVSyncEnabled() const = 0;

	private:
		Window* mWindow;
	};

}
