// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "Core.h"
#include "Events/EventDispatcher.h"
#include "Events/IEventHandler.h"
#include "Logger.h"

// Create derived Zeron application
#define GENERATE_ZERON_APPLICATION(AppClass)\
		int main(int argc, char** argv)\
		{\
			{ZeronEngine::Logger::Init();}\
			ZeronEngine::Application::Create<AppClass>();\
			AppClass::GetInstance()->Run();\
			AppClass::Exit();\
		}\

namespace ZeronEngine
{
	class Application: public IEventHandler
	{
	private:
		static Application * s_Instance;
		std::unique_ptr<EventDispatcher> m_EventDispatcher;
		
	public:
		Application();
		virtual ~Application();

		template<class T> 
		static Application* Create()
		{
			return new T;
		}

		static Application * GetInstance();
		
		void Run();
		static void Exit();
		
		void OnEvent(const Event& e);
	};

}
