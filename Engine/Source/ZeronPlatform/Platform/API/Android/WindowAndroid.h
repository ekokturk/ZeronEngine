// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Platform/Window.h>

#ifdef ZE_PLATFORM_ANDROID

struct ANativeWindow;

namespace Zeron
{
	class SystemEventProcessor;

	class WindowAndroid final : public Window {
	  public:
		WindowAndroid(const WindowConfig& config, ANativeWindow* nativeWindow);
		~WindowAndroid();

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

	  private:
		ANativeWindow* mNativeWindow;
	};

}

#endif