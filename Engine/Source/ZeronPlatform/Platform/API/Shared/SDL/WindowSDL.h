// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_WINDOW_SDL

#include <Platform/Window.h>

union SDL_Event;
struct SDL_Window;

namespace Zeron {
	class WindowSDL final : public Window {
	public:
		WindowSDL(const WindowConfig& config);
		~WindowSDL();

		void SetVisible() override;
		void SetHidden() override;
		
		void SetMinimized() override;
		void SetMaximized() override;
		void SetRestored() override;

		void SetFocused() override;
		void SetAttention() override;

		
		void SetName(const std::string& name) override;
		void SetAspectRatio(int numerator, int denominator) override;
		void SetSize(int width, int height) override;
		void SetSizeLimits(int minWidth, int maxWidth, int minHeight, int maxHeight) override;
		void SetScreenPosition(int posX, int posY) override;

		void SetClipCursor(bool shouldClip) override;

		void* GetApiHandle() const override;
		SystemHandle GetSystemHandle() const override;
		
		int GetCurrentDisplay() const;

	private:
		void _processEvents();
		void _processWindowEvents(const SDL_Event& e);

		void _onFullScreenChangedBorderless() override;
		void _onFullScreenChangedMonitor() override;

		SDL_Window* mWindowSDL;

		static int mWindowSDLCount;
	};

}

#endif
