// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "ZeronEngine.h"

#include "Application.h"
#include "Events/EventTypes.h"
#include "Events/EventDispatcher.h"
#include "Input.h"
#include "Window/WindowModule.h"

namespace ZeronEngine
{
	Application * Application::s_Instance = nullptr;
	
	Application::Application()
		:m_IsRunning(false)
	{
		assert(s_Instance == nullptr);

		s_Instance = this;

		// Event dispatcher for the application
		m_EventDispatcher = std::make_unique<EventDispatcher>();
		
		m_WindowModule = std::make_unique<WindowModule>();
		m_Input = std::make_unique<Input>();

	}
	
	Application::~Application() {}

	Application* Application::Get()
	{
		return s_Instance;
	}

	void Application::Destroy()
	{
		ZERON_LOG_INFO("Application instance destroyed.")
		if (s_Instance)
		{
			s_Instance->Exit();
			delete s_Instance;
			s_Instance = nullptr;
		}
	}


	void Application::Run()
	{
		assert(s_Instance != nullptr);
		
		s_Instance->Init();
		s_Instance->m_IsRunning = true;
		while (s_Instance->m_IsRunning)
		{
			s_Instance->Update();
		}
	}

	void Application::Init()
	{
		m_EventDispatcher->Register<Events::Window_All_Closed>(this, [=](const Events::Window_All_Closed& e)
		{
			Exit();
		});

		
		m_WindowModule->RegisterEvents(*m_EventDispatcher);
		m_WindowModule->Init();
		m_Input->RegisterEvents(*m_EventDispatcher);
		
		OnInit();
	}

	void Application::OnInit(){}

	
	void Application::Update()
	{
		m_WindowModule->Update();

		OnUpdate();
	}
	
	void Application::OnUpdate() {}

	void Application::Exit()
	{
		if(m_IsRunning)
		{
			m_IsRunning = false;
			m_WindowModule->Destroy();
		}
	}




}


