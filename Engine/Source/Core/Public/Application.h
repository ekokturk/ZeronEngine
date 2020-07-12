// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "Core.h"
#include "Logger.h"

// Create derived Zeron application
#define GENERATE_ZERON_APPLICATION(AppClass)\
		int main(int argc, char** argv)\
		{\
			ZeronEngine::Logger::Init();\
			ZeronEngine::Application::Create<AppClass>();\
			AppClass::Run();\
			AppClass::Destroy();\
		}\

namespace ZeronEngine
{
	class EventDispatcher;
	class InputModule;
	class WindowModule;
	
	class Application
	{

	public:
		virtual ~Application();

		template<class T> 
		static Application* Create()
		{
			ZERON_LOG_INFO("Application instance created.")
			return new T;
		}

		static Application * Get();
		static void Destroy();

		
		static void Run();
		void Exit();
		
	protected:
		Application();
		
		/* Initialization for child classes*/
		virtual void OnInit();

		/* Update for child classes*/
		virtual void OnUpdate();
		
	private:
		void Init();
		void Update();

	public:
		EventDispatcher& GetEventDispatcher() { return *m_EventDispatcher; }

		
	private:
		
		static Application* s_Instance;
		std::unique_ptr<EventDispatcher> m_EventDispatcher;
		std::unique_ptr<WindowModule> m_WindowModule;
		std::unique_ptr<InputModule> m_InputModule;

		bool m_IsRunning;

	};

}
