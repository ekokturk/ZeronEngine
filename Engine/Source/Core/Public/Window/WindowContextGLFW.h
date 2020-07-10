// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "WindowContext.h"

struct GLFWwindow;
class EventDispatcher;

/*
 * Window wrapper for GLFW library
 */

namespace ZeronEngine
{

	
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

	private:
		GLFWwindow* m_WindowHandle;
		static int s_WindowCount;
	};

}
