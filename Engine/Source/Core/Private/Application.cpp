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
	template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
	template<class... Ts> overload(Ts...)->overload<Ts...>;
	
	void Application::OnEvent(const Event& e)
	{
		//if( auto pval = std::get_if<EventType::WindowClosed>(&e.m_Data))
		//{
		//	ZERON_LOG("Window Closed")
		//}
		//
		//

		std::visit([&e](auto a)
		{
			ZERON_LOG("Window Closed")
		}, e.m_Data);

	}
}


