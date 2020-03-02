// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "../Public/Application.h"
#include "Events/Event.h"
#include "Events/EventDispatcher.h"
#include "Input.h"
#include "Window.h"
#include <variant>

namespace ZeronEngine
{
	Application * Application::s_Instance = nullptr;
	
	Application::Application()
	{
		assert(s_Instance == nullptr);

		s_Instance = this;

		m_EventDispatcher = std::make_unique<EventDispatcher>();
		m_Window = std::make_unique<Window>();
		m_Input = std::make_unique<Input>();

		// Register Event listeners
		m_EventDispatcher->Register(this);

		// Set event callbacks
		std::function<void(Event&)> eventCallback = [this](Event& e) { m_EventDispatcher->Dispatch(e); };
		m_Input->SetEventCallback(eventCallback);
		m_Window->SetEventCallback(eventCallback);
	}

	Application* Application::GetInstance()
	{
		return s_Instance;
	}

	void Application::DestroyInstance()
	{
		ZERON_LOG_INFO("Application instance destroyed.")
		if (s_Instance != nullptr)
		{
			s_Instance->Exit();
			delete s_Instance;
			s_Instance = nullptr;
		}
	}

	Application::~Application() {}

	void Application::Run()
	{
		Init();
		m_IsRunning = false;
		while (m_IsRunning)
		{
			Update();
		}
	}

	void Application::Init()
	{
		m_Window->Init();

		OnInit();
	}

	void Application::Update()
	{
		m_Window->Update();

		OnUpdate();
	}

	void Application::Exit()
	{
		m_IsRunning = false;
		m_Window->Destroy();
	}
	
	void Application::OnEvent(const Event& e)
	{
		using namespace EventType;
		
		if( e.GetDataRef<WindowClosed>())
		{
			ZERON_LOG("Window Closed")
		}
		else if( const auto data = e.GetDataRef<Custom>())
		{
			ZERON_LOG("Custom Event: {}", (*data).TypeName)
		}
	}
}


