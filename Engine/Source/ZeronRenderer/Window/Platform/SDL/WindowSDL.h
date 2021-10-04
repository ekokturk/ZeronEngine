// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "Window/Window.h"

struct SDL_Window;
union SDL_Event;

namespace Zeron {
	class WindowSDL final : public Window {
	public:
		WindowSDL(const WindowConfig& config);
		~WindowSDL();

		/* Initialize window dependenclies if not initialized yet */
		static bool InitSDL();

		virtual void SetVisible() override;
		virtual void SetHidden() override;
		
		virtual void SetMinimized() override;
		virtual void SetMaximized() override;
		virtual void SetRestored() override;

		virtual void SetFocused() override;
		virtual void SetAttention() override;
		
		virtual void SetName(const std::string& name) override;
		virtual void SetAspectRatio(int numerator, int denominator) override;
		virtual void SetSize(int width, int height) override;
		virtual void SetSizeLimits(int minWidth, int maxWidth, int minHeight, int maxHeight) override;
		virtual void SetScreenPosition(int posX, int posY) override;
		
		virtual void SetFullScreen(bool isFullScreen) override;

		virtual void* GetPlatformHandle() const override;
		
		int GetCurrentDisplay() const;

	private:
		// Events
		void ProcessEvents();
		void ProcessWindowEvents(const SDL_Event& e);

	public:
		void BeginFrame() override;
		void EndFrame() override;
	private:
		SDL_Window* mWindowSDL;

		static int mWindowSDLCount;
	};

}

