// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_WINDOW_SDL
#include <Window/Window.h>

union SDL_Event;
struct SDL_Window;

namespace Zeron {
	class WindowSDL final : public Window {
	public:
		WindowSDL(const WindowConfig& config);
		~WindowSDL();

		bool Init() override;

		void Update() override;

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

		static KeyCode _getKeyCodeSDL(int32_t code);
		static MouseCode _getMouseCodeSDL(int32_t code);

		SDL_Window* mWindowSDL;

		static int mWindowSDLCount;
	};

}

#endif
