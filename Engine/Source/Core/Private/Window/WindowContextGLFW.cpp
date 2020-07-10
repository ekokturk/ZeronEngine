// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "ZeronEngine.h"

#include "Window/WindowContextGLFW.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "Application.h"
#include "Events/EventDispatcher.h"
#include "Events/EventTypes.h"

namespace ZeronEngine
{
	// Count of active GLFW window contexes
	int WindowContextGLFW::s_WindowCount = 0;
	
	bool WindowContextGLFW::ConfigureContext()
	{
		if(s_WindowCount > 0){
			return true;
		}

		// OpenGL context
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Initialize GLFW
		if (glfwInit() == false)
		{
			ZERON_LOG_ERROR("GLFW failed to initialized!")
			return false;
		}
		return true;
	}

	WindowContextGLFW::WindowContextGLFW(WindowProps&& windowProps)
		: WindowContext(std::move(windowProps)),
		m_WindowHandle(nullptr)
	{
		// Create Window
		m_WindowHandle = glfwCreateWindow(m_WindowProps.Width, m_WindowProps.Height, m_WindowProps.Name.c_str(), nullptr, nullptr);
		if (m_WindowHandle == nullptr)
		{
			ZERON_LOG_ERROR("GLFW failed to create window context '{}'!", m_WindowProps.Name)
			WindowContextGLFW::~WindowContextGLFW();
			return;
		}
		glfwMakeContextCurrent(m_WindowHandle);
		// Initialize glad
		if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == false)
		{
			ZERON_LOG_ERROR("GLAD initialization failed for window context '{}'!", m_WindowProps.Name)
			return;
		}

		s_WindowCount++;
		glfwSetWindowUserPointer(m_WindowHandle, this);
	}

	WindowContextGLFW::~WindowContextGLFW()
	{
		glfwDestroyWindow(m_WindowHandle);
		m_WindowProps.EventDispatcher->Remove(this);
		s_WindowCount--;
		if(s_WindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void WindowContextGLFW::Update()
	{
		if(!glfwWindowShouldClose(m_WindowHandle))
		{
			glfwSwapBuffers(m_WindowHandle);
			glfwPollEvents();
		}
	}

	void WindowContextGLFW::Destroy()
	{
		ZERON_LOG("Window '{}' is destroyed...", m_WindowProps.Name)
		WindowContextGLFW::~WindowContextGLFW();
	}


	void WindowContextGLFW::RegisterEvents()
	{

		//// Mouse moved
		//glfwSetCursorPosCallback(m_WindowHandle, [](GLFWwindow* windowGLFW, double posX, double posY)
		//{
		//	auto* window = static_cast<WindowContextGLFW*>(glfwGetWindowUserPointer(windowGLFW));
		//	Events::Mouse_Moved e(static_cast<float>(posX), static_cast<float>(posY));
		//	window->m_WindowProps.EventDispatcher->Dispatch<Events::Mouse_Moved>(e);
		//});

		// Window Closed
		glfwSetWindowCloseCallback(m_WindowHandle, [](GLFWwindow* windowGLFW)
		{
			auto* window = static_cast<WindowContextGLFW*>(glfwGetWindowUserPointer(windowGLFW));
			const Events::Window_Closed e(window);
			window->m_WindowProps.EventDispatcher->Dispatch<Events::Window_Closed>(e);
		});
		
	}

}
