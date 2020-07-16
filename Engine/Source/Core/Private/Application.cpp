// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "ZeronEngine.h"

#include "Application.h"
#include "Events/EventTypes/EventTypes.h"
#include "Events/EventDispatcher.h"
#include "Input/InputModule.h"
#include "Window/WindowGLFW.h"
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

		// Modules
		m_WindowModule = std::make_unique<WindowModule>();
		m_InputModule = std::make_unique<InputModule>();

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
		m_EventDispatcher->Register<Events::Window::MainClose>(this, [=](const Events::Window::MainClose& e)
		{
			Exit();
		});

		// TODO get this from file
		std::shared_ptr<InputMapping> mapping = std::make_shared<InputMapping>();
		
		// Register modules to event dispatcher
		m_InputModule->RegisterEvents(*m_EventDispatcher);
		m_WindowModule->RegisterEvents(*m_EventDispatcher);
		m_InputModule->SetInputMapping(mapping);

		// Initialize modules
		m_WindowModule->Init();

		// To be used on custom applications
		HandleInit();
	}

	void Application::HandleInit(){}
	
	void Application::Update()
	{
		m_WindowModule->Update();

		HandleUpdate();
	}
	
	void Application::HandleUpdate() {}

	void Application::Exit()
	{
		if(m_IsRunning)
		{
			m_IsRunning = false;
			m_WindowModule->Destroy();
		}
	}




}


