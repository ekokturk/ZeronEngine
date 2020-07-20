// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Window/Window.h"
#include "Window/WindowModule.h"


namespace ZeronEngine
{
	WindowProps::WindowProps(const ZeronEngine::EventDispatcher* dispatcher)
		:Name("Zeron Engine"),
		Width(800),
		Height(600),
		IsFullScreen(false),
		IsMinimized(false),
		RefreshRate(60),
		EventDispatcher(dispatcher),
		WidthPrev(Width),
		HeightPrev(Height),
		ScreenPositionPrev(ScreenPosition),
		MousePositionPrev(MousePosition)
	{
	}

	WindowProps::WindowProps(std::string& name, int width, int height, const ZeronEngine::EventDispatcher* dispatcher)
		:Name(name),
		Width(width),
		Height(height),
		IsFullScreen(false),
		IsMinimized(false),
		RefreshRate(60),
		EventDispatcher(dispatcher),
		WidthPrev(Width),
		HeightPrev(Height),
		ScreenPositionPrev(ScreenPosition),
		MousePositionPrev(MousePosition)
	{
	}



	Window::Window(const WindowProps& windowProps)
		:m_WindowProps(windowProps)
	{
	}

	Window::Window(WindowProps&& windowProps)
		:m_WindowProps(std::move(windowProps))
	{
		
	}


	Window::~Window()
	{
	}

}
