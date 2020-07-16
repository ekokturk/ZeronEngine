// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Window/WindowContext.h"
#include "Window/WindowModule.h"


namespace ZeronEngine
{
	WindowProps::WindowProps(const ZeronEngine::EventDispatcher* dispatcher)
		:Name("Zeron Engine"),
		Width(800),
		Height(600),
		IsFullScreen(false),
		RefreshRate(60),
		EventDispatcher(dispatcher),
		WidthPrev(Width),
		HeightPrev(Height)
	{
	}

	WindowProps::WindowProps(std::string& name, int width, int height, const ZeronEngine::EventDispatcher* dispatcher)
		:Name(name),
		Width(width),
		Height(height),
		IsFullScreen(false),
		RefreshRate(60),
		EventDispatcher(dispatcher),
		WidthPrev(Width),
		HeightPrev(Height)
	{
	}



	WindowContext::WindowContext(const WindowProps& windowProps)
		:m_WindowProps(windowProps)
	{
	}

	WindowContext::WindowContext(WindowProps&& windowProps)
		:m_WindowProps(std::move(windowProps))
	{
		
	}


	WindowContext::~WindowContext()
	{
	}

}
