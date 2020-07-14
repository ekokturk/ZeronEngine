// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Window/WindowContext.h"
#include "Window/WindowModule.h"


namespace ZeronEngine
{
	WindowProps::WindowProps(const ZeronEngine::EventDispatcher* dispatcher)
		:Name("Zeron Engine"),
		Width(800),
		Height(600),
		EventDispatcher(dispatcher)
	{
	}

	WindowProps::WindowProps(std::string& name, int width, int height, const ZeronEngine::EventDispatcher* dispatcher)
		:Name(name),
		Width(width),
		Height(height),
		EventDispatcher(dispatcher)
	{
	}

	bool WindowContextHandle::IsValid() const
	{
		return m_WindowCreator && m_WindowCreator->HasWindow(*this);
	}

	WindowContext* WindowContextHandle::Get() const
	{
		if(IsValid())
		{
			return m_WindowCreator->GetWindow(m_HandleID);
		}
		return nullptr;
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
