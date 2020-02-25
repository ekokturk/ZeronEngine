// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "../Public/Application.h"
#include "Events/Event.h"
#include "Events/EventDispatcher.h"

namespace ZeronEngine
{
	Application * Application::s_Instance = nullptr;
	
	Application::Application()
	{
		// If the application instance already created destroy copies
		if (s_Instance != nullptr)
		{
			
			delete this;
			return;
		}
		
		// Initialize app instance
		s_Instance = this;

		std::unique_ptr<EventDispatcher> eventDispatcher = std::make_unique<EventDispatcher>();
		eventDispatcher->Register(this);

		Event event;
		event.m_Type = EEventType::AppStart;
		eventDispatcher->Dispatch(event);
	}

	Application* Application::GetInstance()
	{
		return s_Instance;
	}

	void Application::Destroy()
	{
		if(s_Instance != nullptr)
		{
			delete s_Instance;
			s_Instance = nullptr;
		}
	}


	void Application::OnEvent(const Event& e)
	{
		switch (e.m_Type)
		{
		case EEventType::AppStart:
			ZERON_LOG("App Started")
			break;
			
		default:
			break;
		}
	}
}
