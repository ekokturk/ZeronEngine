// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_WINDOW_GLFW

#include <Platform/Window.h>

class EventDispatcher;
struct GLFWcursor;
struct GLFWmonitor;
struct GLFWwindow;

namespace Zeron {
	
	class WindowGLFW final : public Window {
	public:
		WindowGLFW(const WindowConfig& windowProps, void* userData = nullptr);
		~WindowGLFW();

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
		
		bool IsCursorClipped() const { return mIsCursorClipped; }

		GLFWmonitor* FindCurrentMonitor() const;

	private:
		void _onFullScreenChangedBorderless() override;
		void _onFullScreenChangedMonitor() override;

		GLFWwindow* mWindowGLFW;
		GLFWmonitor* mMonitorGLFW;
		GLFWcursor* mCursorGLFW;

		bool mIsCursorClipped;

		static int mWindowGLFWCount;
	};

}

#endif