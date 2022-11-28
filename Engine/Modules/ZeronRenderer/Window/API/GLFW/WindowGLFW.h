// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Window/Window.h>

class EventDispatcher;
struct GLFWcursor;
struct GLFWmonitor;
struct GLFWwindow;

namespace Zeron {
	
	class WindowGLFW final : public Window {
	public:
		WindowGLFW(const WindowConfig& windowProps);
		~WindowGLFW();

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
		
		// This is needed for input callbacks
		void QueueEvent(std::unique_ptr<WindowEvent> e);
		bool IsCursorClipped() const { return mIsCursorClipped; }

		GLFWmonitor* FindCurrentMonitor() const;

	private:
		void _registerEvents();
		void _onFullScreenChangedBorderless() override;
		void _onFullScreenChangedMonitor() override;

		// Native GLFW window is set to point to WindowGLFW
		static WindowGLFW* _getUserPointerGLFW(GLFWwindow* windowGLFW);

		static KeyCode _getKeyCodeGLFW(int code);
		static MouseCode _getMouseCodeGLFW(int code);
		
		GLFWwindow* mWindowGLFW;
		GLFWmonitor* mMonitorGLFW;
		GLFWcursor* mCursorGLFW;

		bool mIsResizing;
		bool mIsCursorClipped;

		static int mWindowGLFWCount;
	};

}
