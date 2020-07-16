// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "WindowContext.h"

struct GLFWwindow;
struct GLFWmonitor;
struct GLFWcursor;
class EventDispatcher;

namespace ZeronEngine
{
	/*
	 * Window wrapper for GLFW library
	 */

	class WindowContextGLFW : public WindowContext
	{
	public:

		/* Initialize window dependencies if not initialized yet */
		static bool ConfigureContext();
		
		WindowContextGLFW(WindowProps&& windowProps);
		~WindowContextGLFW();

		virtual void Update() override;
		virtual void Destroy() override;
		virtual void RegisterEvents() override;

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
		virtual void SetMousePosition(const Vector2& mousePosition) override;
		
		virtual void SetFullScreen(bool isFullScreen) override;

		GLFWmonitor* GetCurrentMonitor() const;
		
	private:
		GLFWwindow* m_WindowHandle;
		GLFWmonitor* m_MonitorHandle;
		GLFWcursor* m_CursorHandle;
		static int s_WindowCount;
	};

}
