// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "Core.h"
#include "Events/IEventHandler.h"
#include "Logger.h"

// Create derived Zeron application
#define GENERATE_ZERON_APPLICATION(AppClass)\
		int main(int argc, char** argv)\
		{\
			{ZeronEngine::Logger::Init();}\
			ZeronEngine::Application::Create<AppClass>();\
			AppClass::GetInstance()->Run();\
			AppClass::DestroyInstance();\
		}\

namespace ZeronEngine
{
	class EventDispatcher;
	class Window;
	class Input;
	
	class Application: public IEventHandler
	{
	private:
		static Application * s_Instance;
		std::unique_ptr<EventDispatcher> m_EventDispatcher;
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<Input> m_Input;

		bool m_IsRunning;
	public:
		Application();
		virtual ~Application();

		template<class T> 
		static Application* Create()
		{
			ZERON_LOG_INFO("Application instance created.")
			return new T;
		}

		static Application * GetInstance();
		static void DestroyInstance();

		void OnEvent(const Event& e);
		
		void Run();
		void Exit();
		
	protected:
		virtual void OnInit(){};
		virtual void OnUpdate(){};
		
	private:
		void Init();
		void Update();
	};

}
